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
    
    // MAKR: View
    override func viewDidLoad() {
        super.viewDidLoad()
        getStepStatistics()
        getHeartbeatStatistics()
    }
    
    // MARK: Action
    @IBAction fileprivate func tapped(refresh button: UIBarButtonItem) {
        getStepStatistics()
        getHeartbeatStatistics()
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
                DispatchQueue.main.async {
                    self.stepsLabel.text = "\(Int(sum))"
                }
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
                DispatchQueue.main.async {
                    self.bpmLabel.text = "\(Int(average))"
                }
            }
            health.execute(query)
        })
    }
}
