<?php
if ($_SESSION[rank] > 0) {
 if ($_GET[remove_ip]) {
  mysql_query("DELETE FROM ip_list WHERE ip = '$_GET[remove_ip]'");
 }
?>
<div class="title">Letiltási lista</div>
<br />
<br />
<b><u>Letiltott felhasználók listája:</u></b>
<br /><br />
<?php
$query = mysql_query("SELECT * FROM users WHERE banned = '1' ORDER BY name ASC");
$banned_ppl = mysql_num_rows($query);
if ($banned_ppl) {
 while ($sql = mysql_fetch_array($query)) {
  $forumname = $sql[forum_name];
  if (strlen($forumname) < 3) { $forumname = $sql[name]; }
  echo '<img src="images/people.png" alt="" /> <a href="?p=main/userdata&amp;user='.$sql[name].'" class="link"><b>'.text($forumname).'</b></a><br>'."\n";
 }
} else {
 echo '<div class="red_line">Jelenleg nincs letiltott felhasználó!</div>';
}
?>
<br /><br />
<b><u>Letiltott IP címek listája:</u></b>
<br /><br />
<?php
$query = mysql_query("SELECT * FROM ip_list ORDER BY ip ASC");
$banned_ips = mysql_num_rows($query);
if ($banned_ips) {
 while ($sql = mysql_fetch_array($query)) {
  echo '<img src="images/arrow.png" alt="" /> <b>'.$sql[ip].'</b><a href="?p=main/banlist&amp;remove_ip='.$sql[ip].'" class="link">( eltávolítás )</a><br>'."\n";
 }
} else {
 echo '<div class="red_line">Jelenleg nincs letiltott IP cím!</div>';
}
}
?>