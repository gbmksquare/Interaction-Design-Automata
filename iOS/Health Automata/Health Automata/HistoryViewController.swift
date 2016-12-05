//
//  HistoryViewController.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 5..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit
import HealthKit

class HistoryViewController: UIViewController {
    @IBOutlet fileprivate weak var collectionView: UICollectionView!
    @IBOutlet fileprivate weak var tableView: UITableView!
    
    fileprivate var stepStatisticCollection: HKStatisticsCollection? {
        didSet {
            DispatchQueue.main.async { [unowned self] in
                self.tableView.reloadData()
            }
        }
    }
    fileprivate var heartbeatStatisticCollection: HKStatisticsCollection? {
        didSet {
            DispatchQueue.main.async { [unowned self] in
                self.tableView.reloadData()
            }
        }
    }
    
    // MARK: View 
    override func viewDidLoad() {
        super.viewDidLoad()
        collectionView.delegate = self
        collectionView.dataSource = self
        tableView.delegate = self
        tableView.dataSource = self
        getStepStatistics()
        getHeartbeatStatistics()
    }
    
    // MARK: Health
    fileprivate func getStepStatistics(date: Date = Date()) {
        guard HKHealthStore.isHealthDataAvailable() else {
            print("Health store unavailable.")
            return
        }
        
        guard let sampleType = HKSampleType.quantityType(forIdentifier: .stepCount) else {
            print("Step count unavailable.")
            return
        }
        
        let health = HKHealthStore()
        
        health.requestAuthorization(toShare: nil, read: [sampleType], completion: { [unowned self] (isComplete, error) in
            guard isComplete == true && error == nil else {
                print("Unable to get step count data.")
                return
            }
            
            let calendar = Calendar.current
            let start = calendar.startOfDay(for: date)
            
            var dateComponents = DateComponents()
            dateComponents.day = 1
            dateComponents.second = -1
            let end = calendar.date(byAdding: dateComponents, to: start)
            
            let predicate = HKQuery.predicateForSamples(withStart: start, end: end, options: [])
            var intervalComponents = DateComponents()
            intervalComponents.hour = 1
            let query = HKStatisticsCollectionQuery(quantityType: sampleType, quantitySamplePredicate: predicate, options: HKStatisticsOptions.cumulativeSum, anchorDate: Date(), intervalComponents: intervalComponents)
            query.initialResultsHandler = { (query, collection, error) in
                self.stepStatisticCollection = collection
            }
            health.execute(query)
        })
    }
    
    fileprivate func getHeartbeatStatistics(date: Date = Date()) {
        guard HKHealthStore.isHealthDataAvailable() else {
            print("Health store unavailable.")
            return
        }
        
        guard let sampleType = HKSampleType.quantityType(forIdentifier: .heartRate) else {
            print("Heart rate unavailable.")
            return
        }
        
        let health = HKHealthStore()
        
        health.requestAuthorization(toShare: nil, read: [sampleType], completion: { [unowned self] (isComplete, error) in
            guard isComplete == true && error == nil else {
                print("Unable to get heart rate data.")
                return
            }
            
            let calendar = Calendar.current
            let start = calendar.startOfDay(for: date)
            
            var dateComponents = DateComponents()
            dateComponents.day = 1
            dateComponents.second = -1
            let end = calendar.date(byAdding: dateComponents, to: start)
            
            let predicate = HKQuery.predicateForSamples(withStart: start, end: end, options: [])
            var intervalComponents = DateComponents()
            intervalComponents.hour = 1
            let query = HKStatisticsCollectionQuery(quantityType: sampleType, quantitySamplePredicate: predicate, options: HKStatisticsOptions.discreteAverage, anchorDate: Date(), intervalComponents: intervalComponents)
            query.initialResultsHandler = { (query, collection, error) in
                self.heartbeatStatisticCollection = collection
            }
            health.execute(query)
        })
    }
}

extension HistoryViewController: UICollectionViewDelegateFlowLayout {
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumLineSpacingForSectionAt section: Int) -> CGFloat {
        return 4
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumInteritemSpacingForSectionAt section: Int) -> CGFloat {
        return 0
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        return CGSize(width: collectionView.bounds.height - 10, height: collectionView.bounds.height - 10)
    }
    
    func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        let calendar = Calendar.current
        var components = DateComponents()
        components.day = -indexPath.row
        let date = calendar.date(byAdding: components, to: Date())!
        getStepStatistics(date: date)
        getHeartbeatStatistics(date: date)
    }
}

extension HistoryViewController: UITableViewDelegate {
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        
    }
}

extension HistoryViewController: UICollectionViewDataSource {
    func numberOfSections(in collectionView: UICollectionView) -> Int {
        return 1
    }
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return 30
    }
    
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "cell", for: indexPath) as! HistoryCollectionViewCell
        let calendar = Calendar.current
        var components = DateComponents()
        components.day = -indexPath.row
        let date = calendar.date(byAdding: components, to: Date())!
        components = calendar.dateComponents([.day], from: date)
        cell.textLabel.text = "\(components.day!)"
        return cell
    }
}

extension HistoryViewController: UITableViewDataSource {
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 24
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "cell", for: indexPath) as! HistoryTableViewCell
        if let statistics = stepStatisticCollection?.statistics(), statistics.count > indexPath.row {
            let stepStatistic = statistics[indexPath.row]
            let unit = HKUnit(from: "count")
            let steps = Int(stepStatistic.sumQuantity()!.doubleValue(for: unit))
            cell.stepsLabel.text = "\(steps)"
        } else {
            cell.stepsLabel.text = "No Data"
        }
        if let statistics = heartbeatStatisticCollection?.statistics(), statistics.count > indexPath.row {
            let heartbeatStatistic = statistics[indexPath.row]
            let unit = HKUnit(from: "count/min")
            let bpm = Int(heartbeatStatistic.averageQuantity()!.doubleValue(for: unit))
            cell.bpmLabel.text = "\(bpm)"
        } else {
            cell.bpmLabel.text = "No Data"
        }
        
        let dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .short
        dateFormatter.timeStyle = .short
        
        let calendar = Calendar.current
        let origin = calendar.startOfDay(for: Date())
        
        var dateComponents = DateComponents()
        dateComponents.hour = indexPath.row
        let start = calendar.date(byAdding: dateComponents, to: origin)!
        dateComponents.hour = indexPath.row + 1
        let end = calendar.date(byAdding: dateComponents, to: origin)!
        cell.timeLabel.text = "\(dateFormatter.string(from: start))  -  \(dateFormatter.string(from: end))"
        
        return cell
    }
}
