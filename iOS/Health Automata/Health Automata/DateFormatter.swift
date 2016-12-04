//
//  DateFormatter.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 5..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import Foundation

extension DateFormatter {
    class func hkDateFormatter() -> DateFormatter {
        let formatter = DateFormatter()
        formatter.dateStyle = .short
        formatter.timeStyle = .short
        return formatter
    }
}
