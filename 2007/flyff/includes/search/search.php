<script type="text/javascript" language="JavaScript">
function show_all()
{
  var search = document.getElementsByName("search");
  for (var i=1; i < Math.ceil(search.length/2); i++)
  {
    obj      = document.getElementById("tr_" + i);
    obj_line = document.getElementById("tr_line_" + i);
    obj.style.display      = "";
    obj_line.style.display = "";
  }
  var show_all = document.getElementById("tr_0");
  show_all.style.display = "none";
}
</script>
<?php
include 'form.php';
$key = addslashes(htmlspecialchars($_POST['search']));
if (test($key,3,50)) {
 $java = "'";
 echo "\n".'<table cellpadding="0" cellspacing="0" class="table_border">';
 echo "\n".'<tr align="center" class="table_title">';
 echo "\n".'<td width="25%"><b>Találat</b></td>';
 echo "\n".'<td width="35%"><b>Dátum</b></td>';
 echo "\n".'<td width="40%"><b>Idézet</b></td>';
 echo "\n".'</tr>';
 /* Keresés a felhasználók között */
 $sql_query = mysql_query("SELECT name,full_name,forum_name,characters,reg FROM users");
 while ($sql = mysql_fetch_array($sql_query)) {
  $match = 0;
  if (preg_match('/'.$key.'/i',no_format($sql[characters]))) { $match = 1; $short_text = $sql[characters]; }
  if (preg_match('/'.$key.'/i',$sql[name])) { $match = 1; $short_text = $sql[name]; }
  if (preg_match('/'.$key.'/i',$sql[full_name])) { $match = 1; $short_text = $sql[full_name]; }
  if (preg_match('/'.$key.'/i',no_format($sql[forum_name]))) { $match = 1; $short_text = $sql[forum_name]; }
  if ($match == 1) {
   $a = $a + 1;
   if ($a > 20) { $display = 'display: none'; }
   if (strlen(no_format($short_text)) > 50) { $short_text = substr($short_text,0,50).'...'; }
   $mouseover = " onmouseover=\"mouse_over('".$a."')\" onmouseout=\"mouse_out('".$a."')\"";
   $mouseover .= " onClick=\"document.location = '?p=main/userdata&amp;user=".$sql[name]."'\"";
   echo "\n".'<tr name="search" id="tr_line_'.$a.'" class="table_dark_line" style="'.$display.'"><td colspan="3"></td></tr>';
   echo "\n".'<tr name="search" id="tr_'.$a.'" align="center" class="table_field" style="'.$display.'">';
   echo "\n".'<td'.$mouseover.'><a href="?p=main/userdata&amp;user='.$sql[name].'" class="link">Felhasználók</a>';
   echo "\n".'<td'.$mouseover.'><font class="date">'.$sql[reg].'</font>';
   echo "\n".'<td'.$mouseover.'>'.text($short_text);
   echo "\n".'</tr>';
  }
 }
 /* Keresés galériában */
 $sql_query = mysql_query("SELECT name,full_name,forum_name FROM users");
 while ($sql = mysql_fetch_array($sql_query)) {
  $match = 0;
  $kepek = mysql_num_rows(mysql_query("SELECT name FROM images WHERE hidden = 0 AND name = '$sql[name]'"));
  if ($kepek > 0) {
   if (preg_match('/'.$key.'/i',$sql[name])) { $match = 1; $short_text = $sql[name]; }
   if (preg_match('/'.$key.'/i',$sql[full_name])) { $match = 1; $short_text = $sql[full_name]; }
   if (preg_match('/'.$key.'/i',no_format($sql[forum_name]))) { $match = 1; $short_text = $sql[forum_name]; }
  }
  if ($match == 1) {
   $legujabb_kep = mysql_fetch_array(mysql_query("SELECT date FROM images WHERE hidden = 0 AND name = '$sql[name]' ORDER BY id DESC"));
   $a = $a + 1;
   if ($a > 20) { $display = 'display: none'; }
   $short_text .= " képei";
   if (strlen(no_format($short_text)) > 50) { $short_text = substr($short_text,0,50).'...'; }
   $mouseover = " onmouseover=\"mouse_over('".$a."')\" onmouseout=\"mouse_out('".$a."')\"";
   $mouseover .= " onClick=\"document.location = '?p=gallery/gallery&amp;user=".$sql[name]."'\"";
   echo "\n".'<tr name="search" id="tr_line_'.$a.'" class="table_dark_line" style="'.$display.'"><td colspan="3"></td></tr>';
   echo "\n".'<tr name="search" id="tr_'.$a.'" align="center" class="table_field" style="'.$display.'">';
   echo "\n".'<td'.$mouseover.'><a href="?p=gallery/gallery&amp;user='.$sql[name].'" class="link">Galéria</a>';
   echo "\n".'<td'.$mouseover.'><font class="date">'.$legujabb_kep[date].'</font>';
   echo "\n".'<td'.$mouseover.'>'.text($short_text);
   echo "\n".'</tr>';
  }
 }
 /* Keresés galériában dátum szerint */
 $sql_query = mysql_query("SELECT * FROM images WHERE hidden = 0");
 while ($sql = mysql_fetch_array($sql_query)) {
  $match = 0;
  if (preg_match('/'.$key.'/i',$sql[date])) { $match = 1; }
  if ($match == 1) {
   $kep_user = mysql_fetch_array(mysql_query("SELECT forum_name FROM users WHERE name = '$sql[name]'"));
   $owner = $kep_user[forum_name];
   if (strlen($owner) < 3) { $owner = $sql[name]; }
   $a = $a + 1;
   if ($a > 20) { $display = 'display: none'; }
   $short_text = $owner.' képe';
   if (strlen(no_format($short_text)) > 50) { $short_text = substr($short_text,0,50).'...'; }
   if ($_SESSION[rank] > 0) {
    $hidden = 'hidden = 0 AND';
   }
   $page = ceil(mysql_num_rows(mysql_query("SELECT id FROM images WHERE $hidden id >= '$sql[id]'"))/12);
   $mouseover = " onmouseover=\"mouse_over('".$a."')\" onmouseout=\"mouse_out('".$a."')\"";
   $mouseover .= " onClick=\"document.location = '?p=gallery/gallery&amp;page=".$page."'\"";
   echo "\n".'<tr name="search" id="tr_line_'.$a.'" class="table_dark_line" style="'.$display.'"><td colspan="3"></td></tr>';
   echo "\n".'<tr name="search" id="tr_'.$a.'" align="center" class="table_field" style="'.$display.'">';
   echo "\n".'<td'.$mouseover.'><a href="?p=gallery/gallery&amp;page='.$page.'" class="link">Galéria</a>';
   echo "\n".'<td'.$mouseover.'><font class="date">'.$sql[date].'</font>';
   echo "\n".'<td'.$mouseover.'>'.text($short_text);
   echo "\n".'</tr>';
  }
 }
 /* Keresés a hírek között */
 $sql_query = mysql_query("SELECT * FROM news ORDER BY id DESC");
 while ($sql = mysql_fetch_array($sql_query)) {
  $match = 0;
  if (preg_match('/'.$key.'/i',no_format($sql[title]))) { $match = 1; }
  if (preg_match('/'.$key.'/i',no_format($sql[text]))) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[date_text])) { $match = 1; }
  if ($match == 1) {
   $a = $a + 1;
   if ($a > 20) { $display = 'display: none'; }
   $short_text = $sql[text];
   if (strlen(no_format($short_text)) > 50) { $short_text = substr($short_text,0,50).'...'; }
   $mouseover = " onmouseover=\"mouse_over('".$a."')\" onmouseout=\"mouse_out('".$a."')\"";
   $mouseover .= " onClick=\"document.location = '?p=main/home&amp;news_id=".$sql[id]."'\"";
   echo "\n".'<tr name="search" id="tr_line_'.$a.'" class="table_dark_line" style="'.$display.'"><td colspan="3"></td></tr>';
   echo "\n".'<tr name="search" id="tr_'.$a.'" align="center" class="table_field" style="'.$display.'">';
   echo "\n".'<td'.$mouseover.'><a href="?p=main/home&amp;news_id='.$sql[id].'" class="link">Hírek</a>';
   echo "\n".'<td'.$mouseover.'><font class="date">'.$sql[date_text].'</font>';
   echo "\n".'<td'.$mouseover.'>'.smile(text($short_text));
   echo "\n".'</tr>';
  }
 }
 /* Keresés a fórum témákban */
 $sql_query = mysql_query("SELECT * FROM forum ORDER BY id DESC");
 while ($sql = mysql_fetch_array($sql_query)) {
  $match = 0;
  $adatok = mysql_fetch_array(mysql_query("SELECT * FROM forum_themes WHERE id = '$sql[theme_id]'"));
  if (preg_match('/'.$key.'/i',no_format($sql[message]))) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[date_text])) { $match = 1; }
  if ($match == 1) {
   $a = $a + 1;
   if ($a > 20) { $display = 'display: none'; }
   $short_text = $sql[message];
   if (strlen(no_format($short_text)) > 50) { $short_text = substr($short_text,0,50).'...'; }
   $page = ceil(mysql_num_rows(mysql_query("SELECT id FROM forum WHERE id >= '$sql[id]' AND theme_id = '$sql[theme_id]'"))/10);
   $mouseover = " onmouseover=\"mouse_over('".$a."')\" onmouseout=\"mouse_out('".$a."')\"";
   $mouseover .= " onClick=\"document.location = '?p=forum/topic&amp;theme=".$sql[theme_id]."&amp;page=".$page."'\"";
   echo "\n".'<tr name="search" id="tr_line_'.$a.'" class="table_dark_line" style="'.$display.'"><td colspan="3"></td></tr>';
   echo "\n".'<tr name="search" id="tr_'.$a.'" align="center" class="table_field" style="'.$display.'">';
   echo "\n".'<td'.$mouseover.'><a href="?p=forum/topic&amp;theme='.$sql[theme_id].'&amp;page='.$page.'" class="link">Fórum / '.text($adatok[theme_name]).'</a>';
   echo "\n".'<td'.$mouseover.'><font class="date">'.$sql[date_text].'</font>';
   echo "\n".'<td'.$mouseover.'>'.smile(text($short_text));
   echo "\n".'</tr>';
  }
 }
 /* Keresés a szavazások között */
 $sql_query = mysql_query("SELECT * FROM votes ORDER BY id DESC");
 while ($sql = mysql_fetch_array($sql_query)) {
  $match = 0;
  if (preg_match('/'.$key.'/i',$sql[title])) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[answer1])) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[answer2])) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[answer3])) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[answer4])) { $match = 1; }
  if (preg_match('/'.$key.'/i',$sql[answer5])) { $match = 1; }
  if ($match == 1) {
   $a = $a + 1;
   if ($a > 20) { $display = 'display: none'; }
   $short_text = $sql[title];
   if (strlen(no_format($short_text)) > 50) { $short_text = substr($short_text,0,50).'...'; }
   $mouseover = " onmouseover=\"mouse_over('".$a."')\" onmouseout=\"mouse_out('".$a."')\"";
   $mouseover .= " onClick=\"document.location = '?p=vote/votes&amp;id=".$sql[id]."'\"";
   echo "\n".'<tr name="search" id="tr_line_'.$a.'" class="table_dark_line" style="'.$display.'"><td colspan="3"></td></tr>';
   echo "\n".'<tr name="search" id="tr_'.$a.'" align="center" class="table_field" style="'.$display.'">';
   echo "\n".'<td'.$mouseover.'><a href="?p=vote/votes&amp;id='.$sql[id].'" class="link">Szavazások</a>';
   echo "\n".'<td'.$mouseover.'><font class="date">Ismeretlen idõpont (!)</font>';
   echo "\n".'<td'.$mouseover.'>'.smile(text($short_text));
   echo "\n".'</tr>';
  }
 }
 if ($a == 0) {
  $mouseover = " onmouseover=\"mouse_over('0')\" onmouseout=\"mouse_out('0')\"";
  echo "\n".'<tr id="tr_0" align="center" style="background-color: #F6F6F6; height: 30px"><td'.$mouseover.' colspan="3">Nincs találat!</td></tr>';
 } else {
  if ($a > 20) {
   $mouseover = " onmouseover=\"mouse_over('0')\" onmouseout=\"mouse_out('0')\" onClick=\"show_all()\"";
   echo "\n".'<tr id="tr_0" align="center" style="background-color: #F6F6F6; height: 30px"><td'.$mouseover.' colspan="3"><img src="images/reply_down.gif" alt="" /> Az összes mutatása</td></tr>';
  }
 }
 echo "\n".'</table><br />';
}
?>