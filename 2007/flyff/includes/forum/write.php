<?php
if ($_SESSION[name]) {
echo '
<script language="Javascript" type="text/javascript">
function insertSmiley(smiley)
{
  var msg = document.getElementById("message") ;  
  msg.value += smiley ;
}
var date = "'.formed_time(1).'" ;
var time = "'.formed_time(2).'" ;
</script>
';
if ($_GET[quote]) {
 $sql = mysql_fetch_array(mysql_query("SELECT message,name FROM forum WHERE id = '$_GET[quote]'"));
 $user_sql = mysql_fetch_array(mysql_query("SELECT forum_name FROM users WHERE name = '$sql[name]'"));
 $owner = $user_sql[forum_name];
 if (strlen($owner) < 3) { $owner = $sql[name]; }
 $quote = '#quote #b'.no_format($owner).' hozzászólása:#/b'."\n".$sql[message]."\n".'#/quote'."\n\n";
}
?>
<div class="title">Üzenet írása</div>
<br />
<br />
<?php
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">Egy mezõ rosszul lett kitöltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
if ($_GET[answer]) {
 echo '<form action="?a=forum/do_write&amp;p=forum/topic&amp;theme='.$_GET[theme].'&amp;answer='.$_GET[answer].'" method="POST">';
} else {
 echo '<form action="?a=forum/do_write&amp;p=forum/topic&amp;theme='.$_GET[theme].'" method="POST">';
}
?>
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
<a onClick="insertSmiley(':rolleyes')"><img src="images/smiley/rolleyes.gif" alt="" border="0" /></a>&nbsp; 
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
<textarea name="message" id="message" onBlur="test('message',3,2000)" style="width: 100%; height: 300px"><?php echo $quote; ?></textarea>
</td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
<input type="reset" class="button" value="Töröl" />
<input type="button" class="button" onClick="history.back(1)" value="Vissza" />
</form>
<br />
<?php
} else {
?>
<div class="title">Üzenet írása</div>
<br />
<br />
<div class="red_line">Elõbb jelentkezz be!</div>
<br />
<br />
<b><a href="javascript: history.back(1)" class="link">Vissza</a> | <a href="?p=login/index" class="link">Bejelentkezés</a></b>
<?php
}
?>
