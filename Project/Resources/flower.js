var canvas = rootNode;
//canvas.addEventListener("mousedown", doMouseDown, false);
w = canvas.width;
h = canvas.height;
ctx = canvas.getContext('2d');


//global variables
var fps = 30;
var interval = 1000 / fps;
var frame = 0;
var sf = Math.min(w, h) / 1.5; //scale factor
var grd = ctx.createRadialGradient(w / 2, h / 2, 0, w / 2, h / 2, sf * 1.2);
grd.addColorStop(0, "rgba(255,255,255,0.25)");

grd.addColorStop(1, cb(rand(200, 255), rand(200, 255), rand(200, 255), 25));

//flowers!
var num_flowers = 7;
var fs = [];
for (var i = 0; i < num_flowers; i++) {
    fs.push({
        size: rand(1, 10),
        petals: rand(4, 12),
        rot: rand(-5, 5),
        c: [rand(100, 255), rand(100, 255), rand(100, 255)],
        x: w / 2,
        y: h / 2
    });
}

fs.sort(function (a, b) {
    return parseFloat(b.size) - parseFloat(a.size)
});

draw();

function draw() {
    if (stopLoop) {
        return;
    }
    setTimeout(function () {
        window.requestAnimationFrame(draw);
        frame += 1;

        //clear canvas
        ctx.shadowBlur = 0;
        ctx.fillStyle = grd;
        ctx.fillRect(0, 0, w, h);

        for (var i = 0; i < num_flowers; i++) {
            draw_flower(sf * fs[i].size / 10, fs[i].petals * 2, 0, fs[i].rot / 15, fs[i].c, fs[i].x, fs[i].y);
        }


    }, interval);
}

function cb(r, g, b, o) {
    return "rgba(" + r + "," + g + "," + b + "," + o + ")";
}

function add_flower(_x, _y) {
    fs.push({
        size: rand(1, 5),
        petals: rand(4, 12),
        rot: rand(-15, 15),
        c: [rand(100, 255), rand(100, 255), rand(100, 255)],
        x: _x,
        y: _y
    });
    num_flowers++;
}

function draw_flower(_rad, _num_pts, init_angle, spin_vel, c, _x, _y) {

    ctx.shadowBlur = 50;
    ctx.lineWidth = 1;
    ctx.shadowColor = cb(c[0], c[1], c[2], 1);
    ctx.fillStyle = cb(c[0], c[1], c[2], .6);
    c2 = [Math.floor(c[0] / 1.6), Math.floor(c[1] / 1.6), Math.floor(c[2] / 1.6)];

    ctx.strokeStyle = cb(c2[0], c2[1], c2[2], 1);

    var pts = [];
    for (var i = 0; i <= _num_pts; i++) {
        var _a = (360 / _num_pts) * i + init_angle + frame * spin_vel;

        pts.push({x: P2L(_rad, _a).x, y: P2L(_rad, _a).y});
    }

    for (var i = 1; i <= _num_pts; i += 2) {
        idx = i % _num_pts;
        ctx.beginPath();
        ctx.moveTo(_x, _y);
        ctx.bezierCurveTo(_x + pts[i - 1].x, _y + pts[i - 1].y, _x + pts[idx + 1].x, _y + pts[idx + 1].y, _x, _y);
        ctx.stroke();
        ctx.fill();
    }
}

function rand(min, max) {
    return Math.round(Math.random() * (max - min) + min);
}

//polar to linear coordinate transform
function P2L(r, angle) {
    var ret = {x: 0, y: 0};
    ret.x = Math.cos(angle * Math.PI / 180) * r;
    ret.y = Math.sin(angle * Math.PI / 180) * r;
    return (ret);
}

function doMouseDown(event) {
    var totalOffsetX = 0;
    var totalOffsetY = 0;
    var canvasX = 0;
    var canvasY = 0;
    var currentElement = this;

    do {
        totalOffsetX += currentElement.offsetLeft - currentElement.scrollLeft;
        totalOffsetY += currentElement.offsetTop - currentElement.scrollTop;
    }
    while (currentElement = currentElement.offsetParent)

    canvasX = event.pageX - totalOffsetX;
    canvasY = event.pageY - totalOffsetY;

    add_flower(canvasX, canvasY)
}

var stopLoop = false;

function dispose() {
    stopLoop = true;
}

registerJSHandler("dispose", dispose);