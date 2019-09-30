function test(id,min,max)
{
 var text = document.getElementById(id) ;
 var test = text.value ;
 test = test.replace (" ","") ;
 testlen = test.length ;
 textlen = text.value.length ;

 text.style.border = "solid #00FF00 1px" ;

 if (testlen < min)
  text.style.borderColor = "#FF0000" ;
 if (textlen > max)
  text.style.borderColor = "#FF0000" ;
}

function test_mail(id)
{
 var mail = document.getElementById(id) ;

 mail.style.border = "solid #00FF00 1px" ;

 var kukac = mail.value.indexOf("@") ;
 if (kukac < 1)
  mail.style.borderColor = "#FF0000" ;

 var pont = mail.value.indexOf(".") ;
 if (pont == -1)
  mail.style.borderColor = "#FF0000" ;
 if (pont < (kukac+2))
  mail.style.borderColor = "#FF0000" ;

 if (mail.value.length < (pont+3))
  mail.style.borderColor = "#FF0000" ;
 if (mail.value.length > (pont+4))
  mail.style.borderColor = "#FF0000" ;
}

function test_pw(id,pw)
{
 var this_pw = document.getElementById(id) ;
 var real_pw = document.getElementById(pw) ;
 
 this_pw.style.border = "solid #FF0000 1px" ;

 if (this_pw.value == real_pw.value)
  this_pw.style.borderColor = "#00FF00" ;

}

function vote_next(id) {
 var url = window.location.href;
 window.location.href = url + "&smallvote=" + id
}

function all_news() {
 var news      = document.getElementById("all_news");
 var news_link = document.getElementById("all_news_link");
 news.style.display      = "block";
 news_link.style.display = "none";
}

function HideNoticeMessage(opacity) 
{
 var opacity = opacity ;	
 var opacityPercent = opacity * 100 ;
 var oe = document.getElementById('notice');
 oe.setAttribute("style", "opacity:" + opacity + ";") ;
 if (oe.style.setAttribute) 
  oe.style.setAttribute("filter", "alpha(opacity=" + opacityPercent + ");")
 if(opacity > 0.1) 
  window.setTimeout("HideNoticeMessage(" + (opacity-0.1) + ")", 50) ;
 else
 {
  oe.setAttribute("style", "opacity:0;") ;
  oe.style.setAttribute("filter", "alpha(opacity=0);") ;
 }
}

function mouse_over(id) 
{
  document.getElementById('tr_'+id).style.backgroundColor = '#DDDDFF'
}
function mouse_out(id) 
{
  document.getElementById('tr_'+id).style.backgroundColor = '#FFFFFF'
}
