//
// Created by neo on 2019-09-09.
//

#import "JSDocument.h"
#import "JSCanvas.h"
#import "node/canvas/ios/CG/CanvasNodeCG.h"
#import "GraphicsPageManager.h"
#import "GraphicsLayer.h"

using namespace AtomGraphics;

@implementation JSDocument {

}

- (instancetype)init {
    self = [super init];
    if (self) {

    }

    return self;
}


- (id)createElement:(JSValue *)elementType {
    if ([elementType isString] && [[[elementType toString] lowercaseString] isEqualToString:@"canvas"]) {
        std::shared_ptr<Node> node(new CanvasNodeCG());
        JSCanvas *canvasJavaScriptInterface = [[JSCanvas alloc] initWithCanvasNode:node];
        return canvasJavaScriptInterface;
    }

    return nil;
}

- (id)getElementById:(JSValue *)elementID {
    JSCanvas *anInterface = nil;
    if ([elementID isNumber]) {
        anInterface = [self getCanvasNodeWithPageID:[elementID toInt32]];
    }
    return anInterface;
}

- (JSCanvas *)getCanvasNodeWithPageID:(long)pageID {

    JSCanvas *anInterface;

    GraphicsPage *page = GraphicsPageManager::getPageByID(pageID);
    if (page) {
        auto node = page->rootLayer()->getRootNode();
        if (!node) {
            node.reset(new CanvasNodeCG);
            page->rootLayer()->setRootNode(node);
        }

        anInterface = [[JSCanvas alloc] initWithCanvasNode:node];
    }

    return anInterface;
}

@end