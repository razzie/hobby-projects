<?php
if ($_SESSION[rank] > 0) { echo '
<script language="Javascript" type="text/javascript">
function topic_del(id, group)
{
  question = window.confirm("Biztosan törölni akarod ezt a fórumtémát?") ;
  if(question)
    window.location.href = "?a=forum/theme_del&id=" + id + "&group=" + group;
}
function new_topic()
{
 div_link = document.getElementById("new_topic_link");
 div_form = document.getElementById("new_topic_form");

 div_link.style.display = "none";
 div_form.style.display = "";
}
</script>'; }
if (!$_GET[group]) {
 $group = 1;
} else {
 $group = $_GET[group];
}
$sql = mysql_fetch_array(mysql_query("SELECT group_name FROM forum_groups WHERE id = '$group'"));
?>
<div class="title"><?php echo no_format($sql[group_name]); ?></div>
<br />
<br />
<?php
if ($_GET[n] == 'success') {
 $comment = '<div class="green_line" id="notice">A fórumtéma sikeresen létrehozva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">A fórumtéma létrehozása sikertelen volt!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'moved') {
 $comment = '<div class="green_line" id="notice">A fórumtéma sikeresen áthelyezve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
$sql_query = mysql_query("SELECT id FROM forum_themes WHERE group_id = '$group'");
$alltheme = mysql_num_rows($sql_query);
while ($sql = mysql_fetch_array($sql_query)) {
 $vmi = mysql_fetch_array(mysql_query("SELECT name,id,date_text FROM forum WHERE theme_id = '".$sql[id]."' ORDER BY id DESC LIMIT 1"));
 $name = $vmi[name];
 if ($name == '') { $name = '- - -'; }
 mysql_query("UPDATE forum_themes SET last_message = '$name', last_message_id = '$vmi[id]', last_date = '$vmi[date_text]' WHERE id = '$sql[id]' LIMIT 1");
}
echo '<a href="?p=forum/groups" title="Vissza a témacsoportokhoz.." class="link"><b>&laquo; Csoportok</b></a><br /><br />'."\n";
if ($_SESSION[rank] > 0 && $_GET[forum] == 'rename' && $_GET[theme_id]) {
$vmi = mysql_fetch_array(mysql_query("SELECT theme_name FROM forum_themes WHERE id = '".$_GET[theme_id]."' LIMIT 1"));
echo '<form action="?a=forum/theme_rename&amp;group='.$group.'&amp;theme='.$_GET[theme_id].'&amp;p=forum/themes" method="post">
<b>Fórumtéma átnevezése:</b><br />
<input type="text" name="name" id="name" value="'.no_format($vmi[theme_name]).'" onBlur="test('."'".'name'."'".',5,100)" style="width: 150px" />
<input type="submit" class="button" value="Rendben" />
</form><br /><br />';
}
if ($_SESSION[rank] > 0 && $_GET[forum] <> 'rename') {
?>
<div id="new_topic_link">
<a href="javascript:new_topic()" title="Új fórumtéma létrehozása" class="link"><img src="images/reply_down.gif" border="0" alt="" /> <b>Új fórumtéma</b></a><br />
</div>
<div id="new_topic_form" style="display: none">
<?php
echo '<form action="?a=forum/create_topic&amp;p=forum/themes&amp;group='.$group.'" method="post">';
?>
<b>Új téma neve: </b><br />
<input type="text" name="name" style="width: 150px" />&nbsp;<input type="submit" class="button" value="Létrehoz" />
</form>
</div>
<br />
<?php
}
if ($alltheme > 0) {
 echo "\n".'<table cellpadding="0" cellspacing="0" class="table_border">';
 echo "\n".'<tr align="center" class="table_title">';
 echo "\n".'<td width="30%"><b>Téma neve</b></td>';
 echo "\n".'<td width="35%"><b>Utolsó üzenet</b></td>';
 echo "\n".'<td width="23%"><b>Hozzászólások</b></td>';
 echo "\n".'<td width="12%"></td>';
 echo "\n".'</tr>';
 $sql = mysql_query("SELECT * FROM forum_themes WHERE group_id = '$group' ORDER BY last_message_id DESC");
 while($vmi = mysql_fetch_array($sql))
 {
  $i = $vmi[id];
  $count  = mysql_num_rows(mysql_query("SELECT * FROM forum WHERE theme_id = '".$i."'"));
  $java = "'";
  $mouseover = " onmouseover=\"mouse_over('".$i."')\" onmouseout=\"mouse_out('".$i."')\"";
  $torol = '';
  if ($_SESSION[rank] > 1) {
   $torol = '<a href="javascript:topic_del('."'".$i."'".', '."'".$group."'".')" title="Fórumtéma törlése"><img src="images/delete.png" border="0" alt="" /></a>';
  }
  if ($_SESSION[rank] > 0) {
   $torol .= ' <a href="?p=forum/themes&amp;group='.$group.'&amp;forum=rename&amp;theme_id='.$i.'" title="Fórumtéma átnevezése"><img src="images/rename.png" border="0" alt="" /></a> <a href="?p=forum/theme_move&id='.$i.'" title="Fórumtéma áthelyezése"><img src="images/arrow.png" border="0" alt="" /></a>';
  }
  $adatok = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$vmi[owner]'"));
  $owner = $adatok[forum_name];
  if ($owner == "") { $owner = $vmi[owner]; }
  $adatok = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$vmi[last_message]'"));
  $last_message = $adatok[forum_name];
  if ($last_message == "") { $last_message = $vmi[last_message]; }
  $adatok = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$vmi[owner]'"));
  $owner = $adatok[forum_name];
  if (strlen($owner) < 3) { $owner = $vmi[owner]; }
  echo "\n".'<tr class="table_dark_line"><td colspan="4"></td></tr>';
  echo "\n".'<tr id="tr_'.$i.'" align="center" class="table_field">';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=forum/topic&amp;theme='.$i.$java.'">&laquo; <a href="?p=forum/topic&amp;theme='.$i.'" title="A témát létrehozta: '.no_format($owner).'" class="link">'.text($vmi[theme_name]).'</a> &raquo;</td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=forum/topic&amp;theme='.$i.$java.'">&laquo; <a href="?p=main/userdata&amp;user='.$vmi[last_message].'" class="link">'.text($last_message).'</a> &raquo;<br /><font class="date">'.$vmi[last_date].'</font></td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=forum/topic&amp;theme='.$i.$java.'">'.$count.' hozzászólás</td>';
  echo "\n".'<td'.$mouseover.'>'.$torol.'</td>';
  echo "\n".'</tr>';
 }
 echo "\n".'</table>';
} else {
 echo '<div class="red_line">Még nincs létrehozva fórumtéma!</div>';
}
?>
