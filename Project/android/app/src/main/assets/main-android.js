var rootNode = AG.getPageRootNodeById(0);
//const chart0 =  new Van.VanChart({
//        width: rootNode.clientWidth,
//        height: rootNode.clientHeight,
//        devicePixelRatio: window.devicePixelRatio,
//        container: rootNode
//        });
//
//chart0.init(options);
//chart0.render();

let ctx = rootNode.getContext("2d");

var count = 0;
var drawCircle = () => {
  var delta = Math.sin(count);

  ctx.clearRect(0, 0, 300, 300);
  ctx.beginPath();
  ctx.arc(150, 150, 100 + delta * 10, 0, 2 * Math.PI);
  ctx.fill();
  ctx.stroke();

  count+= 0.1;

  requestAnimationFrame(drawCircle);

};

requestAnimationFrame(()=>{
  ctx.fillStyle = "red";
  ctx.strokeStyle = "black";
  ctx.lineWidth = 5;

  drawCircle();
})