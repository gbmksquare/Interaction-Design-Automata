//
//  HistoryTableViewCell.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 6..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit

class HistoryTableViewCell: UITableViewCell {
    @IBOutlet weak var stepsLabel: UILabel!
    @IBOutlet weak var bpmLabel: UILabel!
    @IBOutlet weak var timeLabel: UILabel!
    
    override func awakeFromNib() {
        super.awakeFromNib()
        let view = UIView()
        view.backgroundColor = UIColor.theme
        selectedBackgroundView = view
    }
    
    override func prepareForReuse() {
        super.prepareForReuse()
        stepsLabel.text = ""
        bpmLabel.text = ""
    }
}
