<?php
if ($_SESSION[rank] > 0) { echo '
<script language="Javascript" type="text/javascript">
function group_del(id)
{
  question = window.confirm("Biztosan törölni akarod ezt a fórumcsoportot?") ;
  if(question)
    window.location.href = "?a=forum/group_del&id=" + id ;
}
function new_group()
{
 div_link = document.getElementById("new_group_link");
 div_form = document.getElementById("new_group_form");

 div_link.style.display = "none";
 div_form.style.display = "";
}
</script>'; }
?>
<div class="title">Fórum</div>
<br />
<br />
<?php
if ($_GET[n] == 'success') {
 $comment = '<div class="green_line" id="notice">A témacsoport sikeresen létrehozva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">A témacsoport létrehozása sikertelen volt!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
$sql_query = mysql_query("SELECT id FROM forum_themes");
while ($sql = mysql_fetch_array($sql_query)) {
 $vmi = mysql_fetch_array(mysql_query("SELECT name,id,date_text FROM forum WHERE theme_id = '".$sql[id]."' ORDER BY id DESC LIMIT 1"));
 $name = $vmi[name];
 if ($name == '') { $name = '- - -'; }
 mysql_query("UPDATE forum_themes SET last_message = '$name', last_message_id = '$vmi[id]', last_date = '$vmi[date_text]' WHERE id = '$sql[id]' LIMIT 1");
}
$sql_query = mysql_query("SELECT id FROM forum_groups");
$alltheme = mysql_num_rows($sql_query);
while ($sql = mysql_fetch_array($sql_query)) {
 $vmi = mysql_fetch_array(mysql_query("SELECT * FROM forum_themes WHERE group_id = '$sql[id]' ORDER BY last_message_id DESC LIMIT 1"));
 $name = $vmi[last_message];
 if ($name == '') { $name = '- - -'; }
 mysql_query("UPDATE forum_groups SET last_message = '$name', last_message_id = '$vmi[last_message_id]', last_date = '$vmi[last_date]' WHERE id = '$sql[id]'");
}
if ($_SESSION[rank] > 0 && $_GET[group] == 'rename' && $_GET[group_id]) {
$vmi = mysql_fetch_array(mysql_query("SELECT group_name, description FROM forum_groups WHERE id = '$_GET[group_id]'"));
echo '<form action="?a=forum/group_rename&amp;group='.$_GET[group_id].'&amp;p=forum/groups" method="post">
<b>Témacsoport átnevezése:</b><br />
<input type="text" name="name" id="name" value="'.no_format($vmi[group_name]).'" onBlur="test('."'".'name'."'".',5,100)" style="width: 150px" />&nbsp;<input type="text" name="description" id="description" value="'.no_format($vmi[description]).'" onBlur="test('."'".'description'."'".',5,100)" style="width: 250px" />
<input type="submit" class="button" value="Rendben" />
</form><br /><br />';
}
if ($_SESSION[rank] > 1 && $_GET[group] <> 'rename') {
?>
<div id="new_group_link">
<a href="javascript:new_group()" title="Új témacsoport létrehozása" class="link"><img src="images/reply_down.gif" border="0" alt="" /> <b>Új témacsoport</b></a><br />
</div>
<div id="new_group_form" style="display: none">
<form action="?a=forum/create_group&amp;p=forum/groups" method="post">
<b>Új témacsoport neve: </b><br />
<input type="text" name="name" id="name" onBlur="test('name',5,100)" style="width: 150px" />&nbsp;<input type="text" name="description" id="description" onBlur="test('description',10,200)" style="width: 250px" />&nbsp;<input type="submit" class="button" value="Létrehoz" />
</form>
</div>
<br />
<?php
}
if ($alltheme > 0) {
 echo "\n".'<table cellpadding="0" cellspacing="0" class="table_border">';
 echo "\n".'<tr align="center" class="table_title">';
 echo "\n".'<td width="40%"><b>Csoport neve</b></td>';
 echo "\n".'<td width="35%"><b>Utolsó üzenet</b></td>';
 echo "\n".'<td width="15%"><b>Témák</b></td>';
 echo "\n".'<td width="10%"></td>';
 echo "\n".'</tr>';
 $sql = mysql_query("SELECT * FROM forum_groups ORDER BY last_message_id DESC");
 while($vmi = mysql_fetch_array($sql))
 {
  $i = $vmi[id];
  $count  = mysql_num_rows(mysql_query("SELECT * FROM forum_themes WHERE group_id = '".$i."'"));
  $java = "'";
  $mouseover = " onmouseover=\"mouse_over('".$i."')\" onmouseout=\"mouse_out('".$i."')\"";
  $torol = '';
  if ($_SESSION[rank] > 2) {
   $torol = '<a href="javascript:group_del('."'".$i."'".')" title="Témacsoport törlése"><img src="images/delete.png" border="0" alt="" /></a>';
  }
  if ($_SESSION[rank] > 0) {
   $torol .= ' <a href="?p=forum/groups&amp;group=rename&amp;group_id='.$i.'" title="Témacsoport átnevezése"><img src="images/rename.png" border="0" alt="" /></a>';
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
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=forum/themes&amp;group='.$i.$java.'">&laquo; <a href="?p=forum/themes&amp;group='.$i.'" title="A csoportot létrehozta: '.no_format($owner).'" class="link">'.text($vmi[group_name]).'</a> &raquo;<br /><font class="date">'.text($vmi[description]).'</font></td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=forum/themes&amp;group='.$i.$java.'">&laquo; <a href="?p=main/userdata&amp;user='.$vmi[last_message].'" class="link">'.text($last_message).'</a> &raquo;<br /><font class="date">'.$vmi[last_date].'</font></td>';
  echo "\n".'<td'.$mouseover.' onClick="document.location = '.$java.'?p=forum/themes&amp;group='.$i.$java.'">'.$count.' téma</td>';
  echo "\n".'<td'.$mouseover.'>'.$torol.'</td>';
  echo "\n".'</tr>';
 }
 echo "\n".'</table>';
} else {
 echo '<div class="red_line">Még nincs létrehozva témacsoport!</div>';
}
?>
