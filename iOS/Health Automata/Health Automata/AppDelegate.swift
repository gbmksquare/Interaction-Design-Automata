//
//  AppDelegate.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 4..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        customizeAppearance()
        return true
    }
}

extension AppDelegate {
    fileprivate func customizeAppearance() {
        UINavigationBar.appearance().barStyle = .black
        UINavigationBar.appearance().tintColor = UIColor.orange
        UITabBar.appearance().barStyle = .black
        UITabBar.appearance().tintColor = UIColor.orange
    }
}
