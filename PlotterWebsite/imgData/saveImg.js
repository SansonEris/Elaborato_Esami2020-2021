$("form[name='imgUploader']").on("submit", function(ev) {
  ev.preventDefault(); // Prevent browser default submit.

  var formData = new FormData(this);

  $.ajax({
    url: "PHP/uploadImg.php",
    type: "POST",
    data: formData,
    success: function (msg) {
      alert(msg)
    },
    cache: false,
    contentType: false,
    processData: false
  });

});
