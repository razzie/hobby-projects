<?php
if ($_SESSION[rank] > 0) {
echo '
<script language="Javascript" type="text/javascript">
function insertSmiley(smiley)
{
  var msg = document.getElementById("news") ;  
  msg.value += smiley ;
}
var date = "'.formed_time(1).'" ;
var time = "'.formed_time(2).'" ;
</script>
';
?>
<div class="title">Új hír hozzáadása</div>
<br />
<br />
<?php echo $comment; ?>
<form action="?a=main/do_news_write&amp;p=main/home" method="post">
<b>Hír neve: </b><br />
<input type="text" name="title" id="title" onBlur="test('title',3,200)" style="width: 100%" />
<br />
<br />
<b>Hír szövege: </b><br />
<table width="100%"><tr valign="top">
<td width="30px"></td>
<td width="530px">
<a onClick="insertSmiley(':)')"><img src="images/smiley/smile.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':D')"><img src="images/smiley/biggrin.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':lol')"><img src="images/smiley/laugh.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('xD')"><img src="images/smiley/xd.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('^^')"><img src="images/smiley/happy.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(';)')"><img src="images/smiley/wink.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':P')"><img src="images/smiley/tongue.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('B)')"><img src="images/smiley/cool.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':??')"><img src="images/smiley/huh.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':o')"><img src="images/smiley/ohmy.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':(')"><img src="images/smiley/sad.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('-.-')"><img src="images/smiley/dry.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':@')"><img src="images/smiley/mad.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':rolleyes')"><img src="images/smiley/rolleyes.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':blush')"><img src="images/smiley/blush.gif" alt="" border="0" /></a>&nbsp; 
</td>
</tr><tr valign="top">
<td width="30px">
<a onClick="insertSmiley(' '+date)"><img src="images/date.png" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(' '+time)"><img src="images/time.png" border="0" alt="" /></a>&nbsp; 
<br /><br />
<a onClick="insertSmiley('#b #/b')"><img src="images/bold.png" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('#i #/i')"><img src="images/italic.png" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('#u #/u')"><img src="images/underlined.png" border="0" alt="" /></a>&nbsp;
</td>
<td width="530px">
<textarea name="news" id="news" onBlur="test('news',3,2000)" style="width: 100%; height: 300px"></textarea>
</td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
<input type="reset" class="button" value="Töröl" />
</form>
<br />
<?php
}
?>
