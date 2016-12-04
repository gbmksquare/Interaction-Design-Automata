//
//  HeartbeatViewController.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 4..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit
import HealthKit

class HeartbeatViewController: UITableViewController {
    fileprivate var samples: [HKQuantitySample]?
    
    // MARK: View
    override func viewDidLoad() {
        super.viewDidLoad()
        getHeartbeats()
    }
    
    // MARK: Health
    func getHeartbeats(limit: Int = HKObjectQueryNoLimit, start: Date? = nil, end: Date? = nil) {
        guard HKHealthStore.isHealthDataAvailable() else {
            print("Health store unavailable.")
            return
        }
        
        guard let heartRateSampleType = HKSampleType.quantityType(forIdentifier: .heartRate) else {
            print("Heart rate unavailable.")
            return
        }
        
        let health = HKHealthStore()
        
        health.requestAuthorization(toShare: nil, read: [heartRateSampleType], completion: { [unowned self] (isComplete, error) in
            guard isComplete == true && error == nil else {
                print("Unable to get health data.")
                return
            }
            
            let predicate = HKQuery.predicateForSamples(withStart: start, end: end, options: [])
            let sortDescriptor = NSSortDescriptor(key: HKSampleSortIdentifierStartDate, ascending: false)
            let query = HKSampleQuery(sampleType: heartRateSampleType, predicate: predicate, limit: limit, sortDescriptors: [sortDescriptor], resultsHandler: { (query, samples, error) in
                guard let samples = samples else {
                    print("No results found")
                    return
                }
                
                self.samples = samples as? [HKQuantitySample]
                DispatchQueue.main.async {
                    self.tableView.reloadData()
                }
            })
            health.execute(query)
        })
    }
}

// MARK: Data source
extension HeartbeatViewController {
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return samples?.count ?? 0
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "cell", for: indexPath)
        if let sample = samples?[indexPath.row] {
            let unit = HKUnit(from: "count/min")
            let bpm = sample.quantity.doubleValue(for: unit)
            cell.textLabel?.text = "\(Int(bpm))"
            
            let formatter = DateFormatter.hkDateFormatter()
            let startTime = formatter.string(from: sample.startDate)
            let endTime = formatter.string(from: sample.endDate)
            cell.detailTextLabel?.text = "\(startTime) ~ \(endTime)"
        }
        return cell
    }
}
