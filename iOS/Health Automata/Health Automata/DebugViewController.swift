//
//  DebugViewController.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 4..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit

class DebugViewController: UITableViewController {
    @IBOutlet fileprivate weak var baseUrlTextField: UITextField!
    @IBOutlet fileprivate weak var indicatorLedPowerSwitch: UISwitch!
    @IBOutlet fileprivate weak var heartbeatLedPowerSwitch: UISwitch!
    @IBOutlet fileprivate weak var heartbeatLedBrightnessSlider: UISlider!
    @IBOutlet fileprivate weak var motor1PowerSwitch: UISwitch!
    @IBOutlet fileprivate weak var motor1SpeedSlider: UISlider!
    
    // MARK: View
    override func viewDidLoad() {
        super.viewDidLoad()
        let defaults = UserDefaults.standard
        baseUrlTextField.text = defaults.string(forKey: "baseUrl")
    }
}

// MARK: Action
extension DebugViewController {
    @IBAction fileprivate func changed(sender: UISwitch) {
        switch sender {
        case indicatorLedPowerSwitch: break;
        case heartbeatLedPowerSwitch: break;
        case motor1PowerSwitch: break;
        default: break
        }
    }
    
    @IBAction fileprivate func changed(slider: UISlider) {
        switch slider {
        case heartbeatLedBrightnessSlider: break
        case motor1SpeedSlider: break
        default: break
        }
    }
}

// MAKR: Text field
extension DebugViewController: UITextFieldDelegate {
    func textFieldDidEndEditing(_ textField: UITextField) {
        guard let text = textField.text else { return }
        
        let defaults = UserDefaults.standard
        defaults.set(text, forKey: "baseUrl")
        defaults.synchronize()
    }
    
    override func scrollViewDidScroll(_ scrollView: UIScrollView) {
        baseUrlTextField.resignFirstResponder()
    }
}

// MARK: Table view
extension DebugViewController {
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
    }
}
