<?php
if ($_SESSION[rank] > 0) {
echo '<script type="text/javascript">
function del_forumname() 
{
  question = window.confirm("Biztosan t�r�lni akarod a felhaszn�l� f�rum nev�t?") ;
  if(question)
    window.location.href = "?p=main/userdata&user='.$_GET[user].'&task=del_forumname" ;
}
function del_avatar() 
{
  question = window.confirm("Biztosan t�r�lni akarod a felhaszn�l� avat�rj�t?") ;
  if(question)
    window.location.href = "?p=main/userdata&user='.$_GET[user].'&task=del_avatar" ;
}
function del_signature() 
{
  question = window.confirm("Biztosan t�r�lni akarod a felhaszn�l� f�rum al��r�s�t?") ;
  if(question)
    window.location.href = "?p=main/userdata&user='.$_GET[user].'&task=del_signature" ;
}
function ban_ip_address() 
{
  question = window.confirm("Biztosan letiltja a felhaszn�l� IP c�m�t?") ;
  if(question)
    window.location.href = "?p=main/userdata&user='.$_GET[user].'&task=ban_ip" ;
}
function del_registration() 
{
  question = window.confirm("Biztosan v�glegesen t�rl�d a regisztr�ci�t a rendszerb�l?") ;
  if(question)
    window.location.href = "?p=main/userdata&user='.$_GET[user].'&task=del_reg" ;
}
</script>';
}
$sql = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_GET[user]'"));
if ($sql[name] == $_GET[user]) {
 if ($_SESSION[rank] > $sql[rank]) {
  if ($_GET[status] == '0' || $_GET[status] == '1') {
   mysql_query("UPDATE users SET banned = '$_GET[status]' WHERE name = '$sql[name]'");
  }
  if ($_GET[level] > -1 && $_GET[level] < $_SESSION[rank]) {
   mysql_query("UPDATE users SET rank = '$_GET[level]' WHERE name = '$sql[name]'");
  }
  if ($_GET[task] == "del_forumname") {
   mysql_query("UPDATE users SET forum_name = '' WHERE name = '$sql[name]'");
  }
  if ($_GET[task] == "del_avatar") {
   mysql_query("UPDATE users SET avatar = '' WHERE name = '$sql[name]'");
  }
  if ($_GET[task] == "del_signature") {
   mysql_query("UPDATE users SET signature = '' WHERE name = '$sql[name]'");
  }
  if ($_GET[task] == "del_reg" && $_SESSION[rank] > 1) {
   mysql_query("DELETE FROM users WHERE name = '$sql[name]' LIMIT 1");
  }
  if ($_GET[task] == "ban_ip" && $sql[ip]) {
   mysql_query("DELETE FROM ip_list WHERE ip = '$sql[ip]'");
   mysql_query("INSERT INTO ip_list (ip) VALUES ('$sql[ip]')");
  }
 }
$sql = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_GET[user]'"));
$forumname = $sql[forum_name];
$forum_name = $sql[forum_name];
$signature = $sql[signature];
if ($signature == '') {
 $signature = '-';
}
if ($sql[rank] == 0) { $level = 'Felhaszn�l�'; }
if ($sql[rank] == 1) { $level = 'Moder�tor'; }
if ($sql[rank] == 2) { $level = 'Adminisztr�tor'; }
if ($sql[rank] == 3) { $level = 'Szuperadmin'; }
if ($sql[banned] == 0) {
 $state = 'Enged�lyezett';
} else {
 $state = 'Tiltott';
}
if ($sql[characters]) { $characters = $sql[characters]; } else { $characters = '-'; }
if ($sql[full_name]) { $full_name = $sql[full_name]; } else { $full_name = '-'; }
if (strlen($forumname) < 3) { $forumname = $sql[name]; $forum_name = '-'; }
$kepek = mysql_num_rows(mysql_query("SELECT id FROM images WHERE name = '$sql[name]' AND hidden <> '1'"));
$result = mysql_query("SELECT avatar FROM users WHERE name = '$sql[name]'");
$avatar_img = mysql_result($result, 0);
if ($avatar_img) {
 $avatar = '<br /><br /><br /><img src="includes/forum/avatar.php?name='.$sql[name].'" class="avatar" alt="" />';
}
$last_ip = $sql[ip];
if ($_SESSION[rank] < 1) {
 $last_ip = 'Felhaszn�l�k sz�m�ra nem l�that�.';
}
echo '
<div class="title">'.text($forumname).' adatai</div>
<br />
<br />
'.$comment.'
<table width="100%"><tr>
<td width="200px"><b>Regisztr�ci�s n�v:</b></td>
<td>'.$sql[name].'</td>
</tr><tr>
<td width="200px"><b>Teljes n�v:</b></td>
<td>'.$full_name.'</td>
</tr><tr>
<td width="200px"><b>F�rum n�v:</b></td>
<td>'.text($forum_name).'</td>
</tr><tr>
<td width="200px"><b>F�rum al��r�s:</b></td>
<td>'.smile(text($signature)).'</td>
</tr><tr>
<td width="200px"><b>St�tusz:</b></td>
<td>'.$state.'</td>
</tr><tr>
<td width="200px"><b>Regisztr�ci� id�pontja:</b></td>
<td>'.$sql[reg].'</td>
</tr><tr>
<td width="200px"><b>Utols� IP-c�m:</b></td>
<td>'.$last_ip.'</td>
</tr><tr>
<td width="200px"><b>Utols� friss�t�s:</b></td>
<td>'.$sql[login].'</td>
</tr><tr>
<td width="200px"><b>Szint:</b></td>
<td>'.$level.'</td>
</tr><tr valign="top">
<td width="200px"><b>K�pt�rhely:</b></td>
<td>'.$kepek.' / 500 <a href="?p=gallery/gallery&amp;user='.$sql[name].'" title="'.no_format($forumname).' k�pei"><img src="images/reply_down.gif" border="0" alt="" /></a></td>
</tr><tr valign="top">
<td width="200px"><b>Karakter(ek):</b>'.$avatar.'</td>
<td>'.text($characters).'</td>
</tr></table>
';
if ($_SESSION[rank] > $sql[rank]) {
?>
<br /><br />
<b>Egy�b be�ll�t�sok:</b><hr class="dashed_line" /><br />
<table><tr>
<td width="200px"><b>Szint m�dos�t�s:</b></td>
<td>
<?php
if ($sql[rank] == 0) {
 echo 'Felhaszn�l�';
} else {
 echo '<a href="?p=main/userdata&amp;user='.$sql[name].'&amp;level=0">Felhaszn�l�</a>';
}
if ($_SESSION[rank] > 1) {
 if ($sql[rank] == 1) {
  echo '&nbsp;|&nbsp;Moder�tor';
 } else {
  echo '&nbsp;|&nbsp;<a href="?p=main/userdata&amp;user='.$sql[name].'&amp;level=1">Moder�tor</a>';
 }
}
if ($_SESSION[rank] > 2) {
 if ($sql[rank] == 2) {
  echo '&nbsp;|&nbsp;Adminisztr�tor';
 } else {
  echo '&nbsp;|&nbsp;<a href="?p=main/userdata&amp;user='.$sql[name].'&amp;level=2">Adminisztr�tor</a>';
 }
}
?>
</td>
</tr><tr>
<td width="200px"><b>St�tusz:</b></td>
<td>
<?php
if ($sql[banned] == 0) {
 echo '<a href="?p=main/userdata&amp;user='.$sql[name].'&amp;status=1">Letilt�s</a>';
} else {
 echo '<a href="?p=main/userdata&amp;user='.$sql[name].'&amp;status=0">Enged�lyez�s</a>';
}
?>
</td>
</tr><tr>
<td width="200px"><b>F�rum n�v:</b></td>
<td>
<a href="javascript:del_forumname()">F�rumban l�tsz�d� n�v t�rl�se</a>
</td>
</tr><tr>
</tr><tr>
<td width="200px"><b>Avat�r:</b></td>
<td>
<a href="javascript:del_avatar()">F�rum avat�r t�rl�se</a>
</td>
</tr><tr>
</tr><tr>
<td width="200px"><b>F�rum al��r�s:</b></td>
<td>
<a href="javascript:del_signature()">F�rum al��r�s t�rl�se</a>
</td>
</tr><tr>
<td width="200px"><b>IP c�m:</b></td>
<td>
<a href="javascript:ban_ip_address()">A felhaszn�l� IP c�m�nek tilt�sa</a>
</td>
<?php
if ($_SESSION[rank] > 1) {
echo '
</tr><tr>
<td width="200px"><b>Regisztr�ci�:</b></td>
<td>
<a href="javascript:del_registration()">Regisztr�ci� v�gleges t�rl�se</a>
</td>
';
}
?>
</tr></table><br />
<?php
}
} else {
echo '
<div class="title">Felhaszn�l� adatai</div>
<br />
<br />
<div class="red_line">Nem l�tezik ilyen nev� felhaszn�l�!</div>
';
}
?>