const CANVAS = rootNode,
    CTX = CANVAS.getContext('2d'),
    W = CANVAS.clientWidth,
    H = CANVAS.clientHeight,
    S = Math.min(W, H),
    NUM_CIRCLES = 500,
    ATTRACTION = 0.25,
    SPREAD = 1.5,
    RANGE = 0.9,
    MAX_CIRCLE_SIZE = 3,
    CIRCLES = [];

class Circle {
    constructor() {
        this.pos = new Vector(W / 2, H / 2);
        this.vel = new Vector(Math.random() * SPREAD + RANGE, Math.random() * SPREAD + RANGE);
        this.rot = Math.random() * Math.PI * 2;
        this.color = 'rgba(255,255,255,0.4)';
        this.r = Math.random() * (MAX_CIRCLE_SIZE - 1) + 1;
    }

    update() {
        const ROT_VEL = Math.random() * ATTRACTION;
        const XCoord = this.pos.x - W / 2;
        const YCoord = this.pos.y - H / 2;

        let dx = this.vel.x * Math.cos(this.rot);
        let dy = this.vel.y * Math.sin(this.rot);
        dx -= XCoord / (S / 2);
        dy -= YCoord / (S / 2);
        this.pos.add(dx, dy);

        this.rot += (Math.random() - Math.random()) * ROT_VEL;
    }

    draw() {
        CTX.beginPath();
        CTX.fillStyle = this.color;
        CTX.arc(this.pos.x, this.pos.y, this.r, 0, Math.PI * 2);
        CTX.fill();
        CTX.closePath();
    }
}

class Vector {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    add(a, b) {
        this.x += a;
        this.y += b;
    }
}

function renderCircles() {
    for (let i = 0; i < CIRCLES.length; i++) {
        CIRCLES[i].update();
        CIRCLES[i].draw();
    }
}

var stopLoop = false;

function loop() {
    if (stopLoop) {
        return;
    }
    window.requestAnimationFrame(loop);
    CTX.fillStyle = 'rgba(0,0,0,0.1)';
    CTX.fillRect(0, 0, W, H);
    renderCircles();
}

function createCircles() {
    for (let i = 0; i < NUM_CIRCLES; i++) {
        CIRCLES.push(new Circle());
    }
}

function init() {
    var devicePixelRatio = window.devicePixelRatio;
    CTX.scale(devicePixelRatio, devicePixelRatio);
    createCircles();
    loop();
}

function dispose() {
    stopLoop = true;
}

registerJSHandler("dispose", dispose);

init();