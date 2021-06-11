const svg = d3.select('#svg');

let drawing = false;
let previous_coords = null;
let realtX = document.getElementById("X");
let realtY = document.getElementById("Y");

function draw_point() {

  if (!drawing)
    return;

  const coords = d3.mouse(this);

  svg.append('circle')
    .attr('cx', previous_coords[0])
    .attr('cy', previous_coords[1])
    .attr('r', 5)
    .style('fill', 'black');

  svg.append('circle')
    .attr('cx', coords[0])
    .attr('cy', coords[1])
    .attr('r', 5)
    .style('fill', 'black');


  svg.append('line')
    .attr('x1', previous_coords[0])
    .attr('y1', previous_coords[1])
    .attr('x2', coords[0])
    .attr('y2', coords[1])
    .style('stroke', 'red')
    .style('stroke-width', 2);

  previous_coords = coords;
  realtX.value = Math.round(previous_coords[0]);
  realtY.value = Math.round(previous_coords[1]);

}



svg.on('mousedown', function() {
  previous_coords = d3.mouse(this)
  drawing = true;
});

svg.on('mouseup', () => {
  drawing = false;
});

svg.on('mousemove', draw_point);


function getSvg(){
  //get svg element.
  var svg = document.getElementById("svg");

//get svg source.
  var serializer = new XMLSerializer();
  return serializer.serializeToString(svg);
}

function uploadData(){
  var show = document.getElementById("showData");

  var data = getSvg();

  show.value = data;
  //var jsonData = JSON.stringify(drawingSpots);
  var jsonData = JSON.stringify(data);

  let height = JSON.stringify(document.getElementById("svg").clientHeight);
  let width = JSON.stringify(document.getElementById("svg").clientWidth);


  $.ajax({
    url: "canvasData/getData.php",
    type: "POST",
    data: {jsonData: jsonData, height, width},
    success: function (msg) {
      alert(msg+"canvasData has been written to jsonFile");
    }, error: function(xhr, resp, text) {
      alert(xhr, resp, text);
    }
  });
}

function clearCanvas(){
  var show = document.getElementById("showData");
  svg.selectAll('*').remove();
  show.value = "";
  realtX.value = 0;
  realtY.value = 0;
  var clear = "";
  $.ajax({
    url: "canvasData/getData.php",
    type: "POST",
    data: clear,
    success: function (msg) {
      console.log("DataCleared");
      //alert(msg+"canvasData has been written to jsonFile");
    }, error: function(xhr, resp, text) {
      alert(xhr, resp, text);
    }
  });
}

function download() {
  var data = getSvg();

  var element = document.createElement('a');
  element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(data));
  element.setAttribute('download', "DrawedSVG.svg");

  element.style.display = 'none';
  document.body.appendChild(element);

  element.click();

  document.body.removeChild(element);
}

