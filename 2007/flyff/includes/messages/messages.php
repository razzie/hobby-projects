<?php
 if ($_SESSION[name]) {
?>
<script language="Javascript" type="text/javascript">
function msg_del(id)
{
  question = window.confirm("Biztosan t�r�lni akarod ezt az �zenetet?") ;
  if(question)
    window.location.href = "?a=messages/del&id=" + id ;
}
</script>
<div class="title">�zenetek</div>
<br />
<br />
<?php
if ($_GET[success] == '1') {
 $comment = '<div class="green_line" id="notice">Az �zenet sikeresen meg�rkezett!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[success] == '0') {
 $comment = '<div class="red_line" id="notice">Egy mez� rosszul lett kit�ltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
?>
<a href="?p=messages/write" title="�j �zenet k�ld�se" class="link"><img src="images/reply_down.gif" border="0" alt="" /> <b>�j �zenet</b></a>
&nbsp;|&nbsp;
<a href="?p=messages/messages" title="Legfrissebb �zenetek" class="link"><b>Legfrissebb</b></a>
&nbsp;|&nbsp;
<a href="?p=messages/messages&amp;dir=in" title="Be�rkezett �zenetek" class="link"><b>Be�rkez�</b></a>
&nbsp;|&nbsp;
<a href="?p=messages/messages&amp;dir=out" title="Elk�ld�tt �zenetek" class="link"><b>Elk�ld�tt</b></a>
<br /><br />
<?php
$limit = '';
if ($_GET[dir] == 'in') {
 $settings = "receiver = '$_SESSION[name]' AND del1 <> '1'";
} else {
 if ($_GET[dir] == 'out') {
  $settings = "sender = '$_SESSION[name]' AND del2 <> '1'";
 } else {
  $settings = "(sender = '$_SESSION[name]' AND del1 <> '1') OR (receiver = '$_SESSION[name]' AND del2 <> '1')";
  $limit = 'LIMIT 10';
 }
}
$sql_query = mysql_query("SELECT * FROM messages WHERE $settings ORDER BY id DESC $limit");
$msgs = mysql_num_rows($sql_query);
if ($msgs > 0) {
 echo "\n".'<table cellpadding="0" cellspacing="0" class="table_border">';
 echo "\n".'<tr align="center" class="table_title">';
 echo "\n".'<td width="5%"></td>';
 echo "\n".'<td width="20%"><b>Mappa</b></td>';
 echo "\n".'<td width="25%"><b>K�ld� / c�mzett</b></td>';
 echo "\n".'<td width="35%"><b>T�rgy / d�tum</b></td>';
 echo "\n".'<td width="15%" colspan="3"></td>';
 echo "\n".'</tr>';
 while($sql = mysql_fetch_array($sql_query))
 {
  $i = $sql[id];
  $java = "'";
  if ($sql[sender] == $_SESSION[name]) {
   $mappa = 'Elk�ld�tt';
   $map = 'out';
   $thename = $sql[receiver];
  } else {
   $mappa = 'Be�rkez�';
   $map = 'in';
   $thename = $sql[sender];
  }
  if ($sql[seen] == '1') {
   $info = '<img src="images/read.png" alt="" />';
  } else {
   $info = '';
  }
  $mouseover = " onmouseover=\"mouse_over('".$i."')\" onmouseout=\"mouse_out('".$i."')\"";
  $adatok = mysql_fetch_array(mysql_query("SELECT name,forum_name FROM users WHERE name = '".$thename."' LIMIT 1"));
  $owner = $adatok[forum_name];
  if (strlen($owner) < 3) { $owner = $thename; }
  if ($map == 'in') {
   $reply = '<a href="?p=messages/write&amp;reply='.$i.'" title="V�lasz k�ld�se"><img src="images/reply.png" border="0" alt="" /></a></td>';
  } else {
   $reply = '';
  }
  echo "\n".'<tr class="table_dark_line"><td colspan="7"></td></tr>';
  echo "\n".'<tr id="tr_'.$i.'" align="center" class="table_field">';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=messages/read&amp;id='.$i.$java.'">'.$info.'</td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=messages/read&amp;id='.$i.$java.'">&laquo; <a href="?p=messages/messages&amp;dir='.$map.'" class="link">'.$mappa.' &raquo;</a></td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=messages/read&amp;id='.$i.$java.'">&laquo; <a href="?p=main/userdata&amp;user='.$thename.'" class="link">'.text($owner).'</a> &raquo;</td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=messages/read&amp;id='.$i.$java.'">'.$sql[subject].'<br /><font class="date">'.$sql[date].'</font></td>';
  echo "\n".'<td'.$mouseover.' width="5%">'.$reply;
  echo "\n".'<td'.$mouseover.' width="5%"><a href="javascript:msg_del('."'".$i."'".')" title="�zenet t�rl�se"><img src="images/delete.png" border="0" alt="" /></a>';
  echo "\n".'<td'.$mouseover.' width="5%"><a href="?p=messages/read&amp;id='.$i.'" title="�zenet elolvas�sa"><img src="images/pm.png" border="0" alt="" /></a></td>';
  echo "\n".'</tr>';
 }
 echo "\n".'</table>';
} else {
 echo '<div class="red_line">Nincs �zenet!</div>';
}
}
?>
