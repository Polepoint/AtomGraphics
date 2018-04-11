var rootNode = AG.getRootCanvasNode();

var ctx = rootNode.getContext('2d');
ctx.fillStyle = "rgb(0,255,0)";
ctx.beginPath();
ctx.arc(200, 400, 100, 0, 2 * Math.PI);
ctx.stroke();
ctx.fill();