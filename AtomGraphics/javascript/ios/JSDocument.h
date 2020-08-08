//
// Created by neo on 2019-09-09.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JSExport.h>

@protocol JSDocumentJSExport <JSExport>

- (id)createElement:(JSValue *)elementType;

- (id)getElementById:(JSValue *)elementID;

@end

@interface JSDocument : NSObject <JSDocumentJSExport>

@end