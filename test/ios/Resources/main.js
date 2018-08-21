var rootNode = AG.getPageRootNodeById(0);
demoAnimation(rootNode);
rootNode = AG.getPageRootNodeById(1);
demoAnimation(rootNode);
rootNode = AG.getPageRootNodeById(2);
demoAnimation(rootNode);
rootNode = AG.getPageRootNodeById(3);
demoAnimation(rootNode);

function demoAnimation(rootNode) {
    var zr = zrender.init(rootNode);
    var w = zr.getWidth();
    var h = zr.getHeight();

    var r = 30;
    var circle = new zrender.Circle({
        shape: {
            cx: r,
            cy: h / 2,
            r: r,
        },
        style: {
            fill: '#555',
            stroke: '#FF6EBE',
        },
        silent: true,
    });

    circle.animate('shape', true)
        .when(2000, {
            cx: w - r,
        })
        .when(10000, {
            cx: r,
        })
        .start();

    zr.add(circle);

    circle = new zrender.Circle({
        shape: {
            cx: r,
            cy: h / 4,
            r: r,
        },
        style: {
            fill: '#F0F',
            stroke: '#FF6EBE',
        },
        silent: true,
    });

    circle.animate('shape', true)
        .when(6000, {
            cx: w - r,
        })
        .when(10000, {
            cx: r,
        })
        .start();

    zr.add(circle);


    circle = new zrender.Circle({
        shape: {
            cx: r,
            cy: h * 3 / 4,
            r: r,
        },
        style: {
            fill: '#FF0',
            stroke: '#FF6EBE',
        },
        silent: true,
    });

    circle.animate('shape', true)
        .when(4000, {
            cx: w - r,
        })
        .when(10000, {
            cx: r,
        })
        .start();

    zr.add(circle);

    circle = new zrender.Circle({
        shape: {
            cx: r,
            cy: h / 5,
            r: r,
        },
        style: {
            fill: '#043',
            stroke: '#FF6EBE',
        },
        silent: true,
    });

    circle.animate('shape', true)
        .when(5000, {
            cx: w - r,
        })
        .when(10000, {
            cx: r,
        })
        .start();

    zr.add(circle);

    circle = new zrender.Circle({
        shape: {
            cx: r,
            cy: h * 4 / 5,
            r: r,
        },
        style: {
            fill: '#502',
            stroke: '#FF6EBE',
        },
        silent: true,
    });

    circle.animate('shape', true)
        .when(1000, {
            cx: w - r,
        })
        .when(10000, {
            cx: r,
        })
        .start();


    zr.add(circle);

}