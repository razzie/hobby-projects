<?php
 if ($_SESSION[name]) {
 if ($_GET[reply]) {
  $sql = mysql_fetch_array(mysql_query("SELECT * FROM messages WHERE id = '$_GET[reply]'"));
  $thename = $sql[sender];
 }
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
?>
<div class="title">Üzenet írása</div>
<br />
<br />
<?php echo $comment; ?>
<form action="?a=messages/send" method="POST">
<b>Címzett: </b><br />
<?php
if ($thename) {
 $adatok = mysql_fetch_array(mysql_query("SELECT name,forum_name FROM users WHERE name = '$thename'"));
 $owner = $adatok[forum_name];
 if (strlen($owner) < 3) { $owner = $thename; }
 echo '<select style="width: 100%" disabled>';
 echo '<option>'.no_format($owner).'</option>';
 echo '<input type="hidden" name="name" value="'.$thename.'" />';
} else {
 $vmi_query = mysql_query("SELECT name,forum_name FROM users WHERE name <> '$_SESSION[name]' ORDER BY name ASC");
 echo '<select name="name" style="width: 100%">';
 echo '<option name="name" value=""><< Válassz! >></option>';
 while ($vmi = mysql_fetch_array($vmi_query)) {
  $owner = $vmi[forum_name];
  if (strlen($owner) < 3) { $owner = $vmi[name]; }
  echo '<option name="name" value="'.$vmi[name].'">'.no_format($owner).'</option>';
}
}
?>
</select>
<br />
<br />
<b>Üzenet tárgya: </b><br />
<?php
 if ($_GET[reply]) {
  echo "<input type=\"text\" name=\"subject\" id=\"subject\" onBlur=\"test('subject',0,20)\" style=\"width: 100%\" value=\"Re: ".$sql[subject]."\" />";
 } else {
  echo "<input type=\"text\" name=\"subject\" id=\"subject\" onBlur=\"test('subject',0,20)\" style=\"width: 100%\" />";
 }
?>
<br />
<br />
<b>Üzenet szövege: </b><br />
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
<textarea name="message" id="message" onBlur="test('message',3,2000)" style="width: 100%; height: 300px"></textarea>
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
