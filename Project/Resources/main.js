//VanChart Demo

function redraw(id) {
    if (id !== undefined) {
        switch (id) {
            case 0:
                chart0.render();
                break;
            case 1:
                chart1.render();
                break;
            case 2:
                chart2.render();
                break;
            case 3:
                chart3.render();
                break;
            default:
                break;
        }
    } else {
        chart0 && chart0.render();
        chart1 && chart1.render();
        chart2 && chart2.render();
        chart3 && chart3.render();
    }
}


var options = AG.getOptionsFromBundle();
options = Van.formatOptions2State(options.data);

var chart0;
var chart1;
var chart2;
var chart3;

function drawSingleChart() {
    var rootNode0 = AG.getPageRootNodeById(0);
    chart0 = new Van.VanChart(rootNode0, options);
    chart0.render();
}

function drawCharts() {

    var rootNode0 = AG.getPageRootNodeById(0);
    chart0 = new Van.VanChart(rootNode0, options);
    chart0.render();

    var rootNode1 = AG.getPageRootNodeById(1);
    chart1 = new Van.VanChart(rootNode1, options);
    chart1.render();

    var rootNode2 = AG.getPageRootNodeById(2);
    chart2 = new Van.VanChart(rootNode2, options);
    chart2.render();

    var rootNode3 = AG.getPageRootNodeById(3);
    chart3 = new Van.VanChart(rootNode3, options);
    chart3.render();
}
