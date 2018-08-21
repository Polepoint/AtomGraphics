//demoHelloWorld();
demoAnimation();

function demoHelloWorld() {
    var rootNode = AG.getPageRootNodeById(0);
    var zr = zrender.init(rootNode);
    var w = zr.getWidth();
    var h = zr.getHeight();

    var sun = new zrender.Circle({
        shape: {
            cx: 0,
            cy: 0,
            r: 50,
        },
        style: {
            fill: '#FF904F',
        },
        position: [w / 2, h / 2],
    });
    zr.add(sun);

    var water = new zrender.Rect({
        shape: {
            cx: 0,
            cy: 0,
            width: w,
            height: h / 2,
        },
        style: {
            fill: new zrender.RadialGradient(0.5, -0.1, 1, [
                {
                    offset: 0,
                    color: '#FFB166',
                },
                {
                    offset: 0.2,
                    color: '#D7C467',
                },
                {
                    offset: 1,
                    color: '#37B0FF',
                },
            ]),
        },
        position: [0, h / 2],
    });
    zr.add(water);

    var sky = new zrender.Rect({
        shape: {
            cx: 0,
            cy: 0,
            width: w,
            height: h,
        },
        style: {
            fill: '#D7F9FF',
        },
        zlevel: -1,
    });
    zr.add(sky);
}


function demoAnimation() {
    var rootNode = AG.getPageRootNodeById(0);
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