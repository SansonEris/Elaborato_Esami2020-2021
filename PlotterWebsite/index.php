<!doctype html>
<html class="no-js" lang="">

<head>
  <meta charset="utf-8">
  <title>Web Server</title>
  <meta name="description" content="">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <meta property="og:title" content="">
  <meta property="og:type" content="">
  <meta property="og:url" content="">
  <meta property="og:image" content="">

  <link rel="manifest" href="site.webmanifest">
  <link rel="apple-touch-icon" href="icon.png">
  <!-- Place favicon.ico in the root directory -->

  <link rel="stylesheet" href="css/normalize.css">
  <link rel="stylesheet" href="css/main.css">
  <link rel="stylesheet" href="css/setup.css">
  <link rel="stylesheet" href="css/index.css">
  <link rel="stylesheet" href="css/canvas.css">
  <link href="bootstrap-5.0.0-beta3-dist/css/bootstrap.min.css" media="all" type="text/css" rel="stylesheet">

  <meta name="theme-color" content="#fafafa">

  <script src="https://d3js.org/d3.v4.min.js">
  <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js" type="text/javascript"></script>
  <script src="js/jQuery.js"></script>


</head>

<body>

  <!--header-->
  <?php include 'html/navBar.html';?>

  <!--canvas and img-->
  <?php include 'html/canvasAndImg.html';?>

  <!--footer-->
  <?php include 'html/footer.html';?>

  <script src="js/vendor/modernizr-3.11.2.min.js"></script>
  <script src="js/plugins.js"></script>

  <script src="bootstrap-5.0.0-beta3-dist/js/bootstrap.min.js"></script>

  <!-- Google Analytics: change UA-XXXXX-Y to be your site's ID. -->
  <script>
    window.ga = function () { ga.q.push(arguments) }; ga.q = []; ga.l = +new Date;
    ga('create', 'UA-XXXXX-Y', 'auto'); ga('set', 'anonymizeIp', true); ga('set', 'transport', 'beacon'); ga('send', 'pageview')
  </script>
  <script src="https://www.google-analytics.com/analytics.js" async></script>

  <script src="imgData/saveImg.js"></script>
  <script src="canvasData/canvasImg.js"></script>

</body>

</html>
