//
// Created by neo on 2019-09-05.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "CollectionViewCell.h"

@interface CollectionViewCell ()

@property(nonatomic, strong) UILabel *titleView;

@end

@implementation CollectionViewCell {

}

- (void)setTitle:(NSString *)title {
    _title = [title mutableCopy];
    self.titleView.text = _title;
}

- (UILabel *)titleView {
    if (!_titleView) {
        _titleView = [[UILabel alloc] initWithFrame:self.bounds];
        _titleView.backgroundColor = [UIColor whiteColor];
        _titleView.textColor = [UIColor purpleColor];
        _titleView.textAlignment = NSTextAlignmentCenter;
        _titleView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
        _titleView.layer.cornerRadius = 10;
        _titleView.layer.borderColor = [UIColor purpleColor].CGColor;
        _titleView.layer.borderWidth = 5;
        _titleView.clipsToBounds = YES;
        [self addSubview:_titleView];
    }

    return _titleView;
}


@end