<div class="title">Felhasználók</div><br />
<?php
$i = 0;
$time = date("YmdHi");
$all_pages = mysql_num_rows(mysql_query("SELECT name FROM users"));
$pages = ceil($all_pages/40);
$page = $_GET[page];
if ($page == 0) { $page = 1; }
$sql = mysql_query("SELECT name,forum_name,ip,login2 FROM users ORDER BY name ASC LIMIT ".(($page-1)*40).", 40");
$all = mysql_num_rows($sql);
$page_minus = $page - 2;
if ($page_minus < 1) { $page_minus = 1; }
$page_plus = $page + 2;
if ($page_plus > $pages) { $page_plus = $pages; }
/* Oldalak számozása */
echo '<div>';
if ($page > 1) {
 echo '&nbsp;<div class="page_box"><a href="?p=main/users&amp;page=1" class="link"><<</a></div>';
 echo '&nbsp;<div class="page_box"><a href="?p=main/users&amp;page='.($page-1).'" class="link"><</a></div>';
}
for ($i=$page_minus; $i<=$page_plus; $i++) {
 echo '&nbsp;<div class="page_box"><a href="?p=main/users&amp;page='.$i.'" class="link">'.$i.'</a></div>';
}
if ($pages > $page) {
 echo '&nbsp;<div class="page_box"><a href="?p=main/users&amp;page='.($page+1).'" class="link">></a></div>';
 echo '&nbsp;<div class="page_box"><a href="?p=main/users&amp;page='.$pages.'" class="link">>></a></div>';
}
echo '</div><br /><br />';
?>
<table width="100%"><tr>
<td width="50%" valign="top">
<?php
$fele = ceil($all/2);
$i = 0;
while ($vmi = mysql_fetch_array($sql))
{
 $name = $vmi[forum_name];
 if ($name == "") { $name = $vmi[name]; }
 if ($fele == $i) { echo '</td><td width="50%" valign="top">'."\n"; }
 $ip = $vmi[ip];
 $logintime = $vmi[login2];
 if (($logintime+3) > $time) {
  $data = ' - <font style="color: #009900"><b>Bejelentkezve</b></font>';
 } else {
  if ($logintime == "0") {
   $data = ' - Kijelentkezett';
  } else {
   if ($ip == "") {
    $data = ' - <font style="color: #990000"><b>Sosem lépett be</b></font>';
   } else {
    $data = ' - Nincs az oldalon';
   }
  }
 }
 $data .= '<br />';
 $i = $i + 1;
 echo '<img src="images/people.png" alt="" />&nbsp;<a href="?p=main/userdata&amp;user='.$vmi[name].'" class="link"><b>'.text($name).'</b></a>'.$data."\n";
}
echo '</td></tr></table>'."\n";
?>
