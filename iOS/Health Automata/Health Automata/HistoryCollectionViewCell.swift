//
//  HistoryCollectionViewCell.swift
//  Health Automata
//
//  Created by BumMo Koo on 2016. 12. 6..
//  Copyright © 2016년 BumMo Koo. All rights reserved.
//

import UIKit

class HistoryCollectionViewCell: UICollectionViewCell {
    @IBOutlet weak var roundView: UIView!
    @IBOutlet weak var textLabel: UILabel!
    
    override func awakeFromNib() {
        super.awakeFromNib()
        roundView.layer.cornerRadius = roundView.bounds.width / 2
        roundView.layer.borderWidth = 3
        roundView.layer.borderColor = UIColor.theme.cgColor
        textLabel.text = ""
    }
    
    override func prepareForReuse() {
        super.prepareForReuse()
        textLabel.text = ""
    }
}
