//
//  StatusViewController.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 6..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit
import HealthKit

class StatusViewController: UIViewController {
    @IBOutlet fileprivate weak var stepsLabel: UILabel!
    @IBOutlet fileprivate weak var bpmLabel: UILabel!
    
    fileprivate var steps: Int = 0 {
        didSet {
            DispatchQueue.main.async { [unowned self] in
                self.stepsLabel.text = "\(self.steps)"
            }
        }
    }
    fileprivate var bpm: Int = 0 {
        didSet {
            DispatchQueue.main.async { [unowned self] in
                self.bpmLabel.text = "\(self.bpm)"
            }
        }
    }
    
    private var dataFetchCount = 0
    
    // MAKR: View
    override func viewDidLoad() {
        super.viewDidLoad()
        getStepStatistics()
        getHeartbeatStatistics()
    }
    
    // MARK: Action
    private func sendDataIfNeeded() {
        dataFetchCount += 1
        if dataFetchCount == 2 {
            dataFetchCount = 0
            API.sendData(steps: steps, bpm: bpm)
        }
    }
    
    @IBAction fileprivate func tapped(refresh button: UIBarButtonItem) {
        getStepStatistics()
        getHeartbeatStatistics()
    }
    
    @IBAction fileprivate func tapped(settings button: UIBarButtonItem) {
        let alert = UIAlertController(title: "Connect to Raspberry Pi", message: "Enter local address of Raspberry Pi that is connected to Arduino.", preferredStyle: .alert)
        let confirm = UIAlertAction(title: "Confirm", style: .default, handler: { _ in
            let baseUrlTextField = alert.textFields?.first!
            if let address = baseUrlTextField?.text, address.characters.count > 0 {
                let defaults = UserDefaults.standard
                defaults.set(address, forKey: "baseUrl")
                defaults.synchronize()
            }
        })
        let cancel = UIAlertAction(title: "Cancel", style: .cancel, handler: nil)
        alert.addAction(confirm)
        alert.addAction(cancel)
        alert.addTextField { (textField) in
            textField.placeholder = "Example: 192.168.0.45"
            textField.text = UserDefaults.standard.string(forKey: "baseUrl")
        }
        present(alert, animated: true, completion: nil)
    }
    
    // MARK: Health
    fileprivate func getStepStatistics() {
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
            
            let end = Date()
            var dateComponents = DateComponents()
            dateComponents.hour = -1
            let calendar = Calendar.current
            let start = calendar.date(byAdding: dateComponents, to: end)
            
            let predicate = HKQuery.predicateForSamples(withStart: start, end: end, options: [])
            var intervalComponents = DateComponents()
            intervalComponents.hour = 1
            let query = HKStatisticsCollectionQuery(quantityType: sampleType, quantitySamplePredicate: predicate, options: HKStatisticsOptions.cumulativeSum, anchorDate: Date(), intervalComponents: intervalComponents)
            query.initialResultsHandler = { (query, collection, error) in
                let unit = HKUnit(from: "count")
                let sum = collection?.statistics().first?.sumQuantity()?.doubleValue(for: unit) ?? 0
                self.steps = Int(sum)
                self.sendDataIfNeeded()
            }
            health.execute(query)
        })
    }
    
    fileprivate func getHeartbeatStatistics() {
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
            
            let end = Date()
            var dateComponents = DateComponents()
            dateComponents.hour = -1
            let calendar = Calendar.current
            let start = calendar.date(byAdding: dateComponents, to: end)
            
            let predicate = HKQuery.predicateForSamples(withStart: start, end: end, options: [])
            var intervalComponents = DateComponents()
            intervalComponents.hour = 1
            let query = HKStatisticsCollectionQuery(quantityType: sampleType, quantitySamplePredicate: predicate, options: HKStatisticsOptions.discreteAverage, anchorDate: Date(), intervalComponents: intervalComponents)
            query.initialResultsHandler = { (query, collection, error) in
                let unit = HKUnit(from: "count/min")
                let average = collection?.statistics().first?.averageQuantity()?.doubleValue(for: unit) ?? 0
                self.bpm = Int(average)
                self.sendDataIfNeeded()
            }
            health.execute(query)
        })
    }
}
