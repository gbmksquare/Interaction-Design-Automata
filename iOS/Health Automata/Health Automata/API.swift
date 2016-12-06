//
//  API.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 6..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import Foundation
import Alamofire

class API {
    class var baseUrl: String {
        let defaults = UserDefaults.standard
        let url = "http://" + defaults.string(forKey: "baseUrl")! + ":8080"
        return url
    }
    
    class func sendData(steps: Int, bpm: Int) {
        let url = API.baseUrl + "/automata"
        let parameters = ["step": steps, "bpm": bpm]
        let _ = Alamofire.request(url, method: .post, parameters: parameters).responseJSON { (response) in
            switch response.result {
            case .success(let value):
                print(value)
            case .failure(let error):
                print(error.localizedDescription)
            }
        }
    }
}
