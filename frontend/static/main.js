$(document).ready(function(){
  console.log('loaded');
  var soundFx = $('#sound');
  var togg = 0;
  var sock = io.connect("http://"+ document.domain + ':' + location.port);
  sock.on('message', function(m){
    if (m["e1"] == "1"){$("#e1").addClass("s_on");}
    if (m["e2"] == "1"){$("#e2").addClass("s_on");}
    if (m["e3"] == "1"){$("#e3").addClass("s_on");}
    if (m["e4"] == "1"){$("#e4").addClass("s_on");}
    if (m["e1"] == "0"){$("#e1").removeClass("s_on");}
    if (m["e2"] == "0"){$("#e2").removeClass("s_on");}
    if (m["e3"] == "0"){$("#e3").removeClass("s_on");}
    if (m["e4"] == "0"){$("#e4").removeClass("s_on");}
    if (m["tamper"] == "1"){$(".tamper-alert").css("visibility", "visible");$("#tamp-master").prop("checked", true);soundFx[0].play();}
    if (m["tamper"] == "0"){$(".tamper-alert").css("visibility", "hidden");soundFx[0].pause();soundFx[0].currentTime=0;}
    if (m["bstatus"] == "1"){$(".loader").css("visibility", "hidden");}
    if (m["bstatus"] == "0"){$(".loader").css("visibility", "visible");}
  });
  $("#master").change(function(){
    if($(this).is(":checked")){
      sock.send("allt0");
    }else{
      sock.send("allt1");
    }
  });
  $("#tamp-master").change(function(){
      sock.send("allt1");
  });
  $("#ac").change(function(){
    if($(this).is(":checked")){
      sock.send("act1");
    }else{
      sock.send("act0");
    }
  });
});
