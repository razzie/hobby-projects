<?php
session_start();
include "includes/connect.php";
if ($mysql !== "error") {
include "includes/stats.php";
include "includes/functions.php";
$_SESSION[session] = 1;
if (!$_GET[a] && !$_GET[p]) {
 header("Location: ?p=main/home");
 exit;
}
if ($_GET[a]) {
 include "includes/" . $_GET[a] . ".php";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="hu" lang="hu">
<head>
 <title>Magyar Flyff gamerek oldala</title>
 <meta name="generator" content="handmade" />
 <meta name="author" content="Görzsöny Gábor" />
 <meta name="keywords" content="flyff,magyar,fórum,szavazás,hírek,képek,portál,fly for fun,leírás,információ,segítség" />
 <meta name="robots" content="index,follow" />
 <meta name="revisit-after" content="1 day" />
 <meta http-equiv="Content-Language" content="hu" />
 <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-2" />
 <link rel="StyleSheet" href="style/style.css" type="text/css" />
 <link rel="shortcut icon" href="images/icon.ico" />
 <script language="Javascript" type="text/javascript" src="scripts/scripts.js"> </script>
</head>
<body bgcolor="#C0C0C0" link="#000000" alink="#DDDDDD"><center>
<table cellspacing="0" cellpadding="0" border="0">
<tr><td class="shadow_top_left"></td><td colspan="3" class="shadow_top"></td><td class="shadow_top_right"></td></tr>
<tr style="height: 140px">
<td rowspan="3" class="shadow_left"></td>
<td colspan="3" align="right" valign="top" class="logo">
<?php
if ($_SESSION[name]) {
 $pm = mysql_num_rows(mysql_query("SELECT * FROM messages WHERE receiver = '$_SESSION[name]' AND seen <> '1'"));
 if ($pm > 0) {
  echo '<br /><a href="?p=messages/messages">'.$pm.' új üzenet</a> &nbsp;';
 }
}
?>
</td>
<td rowspan="3" class="shadow_right"></td>
</tr>
<tr valign="top" align="left" style="height: 500px">
<td style="width: 150px; background-color: #F0F0F0">
<?php
if (!$_SESSION[name]) {
echo '
<div class="menu_title">Profil</div>
<a href="?p=login/index" class="menuitem_left">Bejelentkezés</a>
<a href="?p=main/forgotten_password" class="menuitem_left">Elfelejtett jelszó</a>
<a href="?p=main/reg" class="menuitem_left">Regisztráció</a>
<a href="?p=main/help" class="menuitem_left">Segítség</a>
<a href="?p=main/rules" class="menuitem_left">Szabályzat</a>
';
} else {
$sql = mysql_fetch_array(mysql_query("SELECT name,forum_name,reg,rank,ip FROM users WHERE name = '$_SESSION[name]' LIMIT 1"));
$forumname = $sql[forum_name];
if (strlen($forumname) < 3) { $forumname = $_SESSION[name]; }
echo '
<div class="menu_title"><a href="?p=main/userdata&amp;user='.$_SESSION[name].'" class="link">'.text($forumname).'</a></div>
<a href="?a=login/logout&amp;p=main/home" class="menuitem_left">Kijelentkezés</a>
<a href="?p=main/options" class="menuitem_left">Beállítások</a>
<a href="?p=messages/messages" class="menuitem_left">Üzenetek</a>
<a href="?p=main/stats" class="menuitem_left">Statisztika</a>
<a href="?p=main/help" class="menuitem_left">Segítség</a>
<a href="?p=main/rules" class="menuitem_left">Szabályzat</a>
';
if ($_SESSION[rank] > 0) {
 echo '<a href="?p=main/banlist" class="menuitem_left">Letiltási lista</a>';
}
echo '
<div class="menu_title">Információ</div>
<div class="menu_box">
';
$kepek = mysql_num_rows(mysql_query("SELECT id FROM images WHERE name = '$_SESSION[name]' AND hidden <> '1'"));
if ($sql[rank] == 0) { $level = 'Felhasználó'; }
if ($sql[rank] == 1) { $level = 'Moderátor'; }
if ($sql[rank] == 2) { $level = 'Adminisztrátor'; }
if ($sql[rank] == 3) { $level = 'Szuperadmin'; }
echo '
<b>Felhasználónév:</b><br />
 - '.$_SESSION[name].'<br />
<b>IP cím:</b><br />
 - '.$sql[ip].'<br />
<b>Regisztráció:</b><br />
 - '.$sql[reg].'<br />
<b>Szint:</b><br />
 - '.$level.'
<b>Képtárhely:</b><br />
 - '.$kepek.' / 500 <a href="?p=gallery/gallery&amp;user='.$_SESSION[name].'" title="Képeim mutatása"><img src="images/reply_down.gif" border="0" alt="" /></a>
</div>
';
}
if ($_GET[smallvote]) {
 $query = mysql_query("SELECT * FROM votes WHERE id = '$_GET[smallvote]'");
} else {
 $query = mysql_query("SELECT * FROM votes ORDER BY id DESC LIMIT 1");
}
$votes = mysql_num_rows($query);
$sql = mysql_fetch_array($query);
if ($votes) {
 $other_vote_1 = mysql_fetch_array(mysql_query("SELECT id FROM votes WHERE id = '".($sql[id]-1)."'"));
 $other_vote_2 = mysql_fetch_array(mysql_query("SELECT id FROM votes WHERE id = '".($sql[id]+1)."'"));
 if ($other_vote_1[id]) {
  $vote_prev = '<a href="javascript:vote_next('.$other_vote_1[id].')" class="link" title="Előző szavazás">&laquo;</a>';
 } else {
  $vote_prev = '&nbsp;';
 }
 if ($other_vote_2[id]) {
  $vote_next = '<a href="javascript:vote_next('.$other_vote_2[id].')" class="link" title="Következő szavazás">&raquo;</a>';
 } else {
  $vote_next = '&nbsp;';
 }
}
?>
<div class="menu_title"><?php echo $vote_prev; ?> Szavazás <?php echo $vote_next; ?></div>
<div class="menu_box">
<?php
if ($votes) {
 echo '<a href="?p=vote/votes&amp;id='.$sql[id].'" class="link"><b>'.text($sql[title]).'</b></a><br /><br />';
 if (substr_count($sql[people], '*'.$_SESSION[name].'*') == 0 && $_SESSION[name]) {
  for ($i=1; $i<=8; $i++)
  {
   if ($sql['answer'.$i]) {
    echo '<a href="?a=vote/voting&amp;id='.$sql[id].'&amp;answer='.$i.'&amp;p=vote/votes">'.text($sql['answer'.$i]).'</a><hr class="dashed_line" />';
   }
  }
 } else {
  for ($i=1; $i<=8; $i++)
  {
   if ($sql['answer'.$i]) { $answers += 1; }
   $count += $sql['count'.$i];
  }
  for ($i=1; $i<=8; $i++)
  {
   if ($sql['answer'.$i]) {
    if ($sql['count'.$i] == 0) {
     $length = 0;
     $percent = 0;
    } else {
     $length = ceil($sql['count'.$i]/$count*100);
     $percent = round($sql['count'.$i]/$count*100, 1);
    }
    echo '&nbsp;'.text($sql['answer'.$i]).'<br />'."\n";
    echo '<div style="width: 100%">'."\n";
    echo '<div class="vote"><div class="vote_green" style="width: '.$length.'px"></div></div>'."\n";
    echo '<div style="float: right">&nbsp;<a title="'.$sql['count'.$i].' szavazat" class="percent"> '.$percent.'%</a></div><br />'."\n";
    echo '</div>'."\n";
   }
  }
 }
} else {
 echo '<center>Nincs szavazás.</center>';
}
?>
</div>
<div class="menu_title">Legújabb tagok</div>
<div class="menu_box">
<?php
$query = mysql_query("SELECT name,forum_name,reg, date_format(reg,'Y. m. d. H:i') FROM users ORDER BY reg DESC LIMIT 3");
while ($sql = mysql_fetch_array($query))
{
 $name = $sql[forum_name];
 if (strlen($name) < 3) { $name = $sql[name]; }
 echo '<img src="images/people.png" alt="" />&nbsp;<b><a href="?p=main/userdata&amp;user='.$sql[name].'" class="link">'.text($name).'</a></b><br />'."\n";
}
?>
</div>
</td>
<td align="center" style="width: 600px; background-color: #FFFFFF">
<br />
<div style="width: 560px; text-align: left">
<?php
if ($_GET[p]) {
 $php_exist = @fopen("includes/" . $_GET[p] . ".php", "r");
 if ($php_exist)
 {
  fclose($php_exist);
  include "includes/" . $_GET[p] . ".php";
 } else {
  echo '<div class="title">Hiba</div><br /><br /><div class="red_line">A kért oldal nem jeleníthető meg (404)</div>';
 }
}
?>
</div>
</td>
<td style="width: 150px; background-color: #F0F0F0">
<div class="menu_title">Egyéb</div>
<a href="?p=main/home" class="menuitem_right">Nyitóoldal</a>
<a href="?p=forum/groups" class="menuitem_right">Fórum</a>
<a href="?p=vote/votes" class="menuitem_right">Szavazások</a>
<a href="?p=gallery/gallery" class="menuitem_right">Galéria</a>
<a href="?p=search/form" class="menuitem_right">Keresés</a>
<a href="?p=itemlist/search" class="menuitem_right">Tárgylista</a>
<a href="?p=main/users" class="menuitem_right">Felhasználók</a>
<a href="?p=main/contact" class="menuitem_right">Kapcsolat</a>
<div class="menu_title">Hírek</div>
<div class="menu_box">
<?php
$query = mysql_query("SELECT * FROM news ORDER BY id DESC");
$news = mysql_num_rows($query);
$i = 0;
if ($news > 0) {
 while($sql = mysql_fetch_array($query)) {
  $i = $i + 1;
  if ($i == 6) { echo '<div id="all_news" style="display: none">'; }
  echo '<img src="images/arrow.png" alt="" />&nbsp;<a href="?p=main/home&amp;news_id='.$sql[id].'#jump">'.text($sql[title]).'</a><br />'."\n";
 }
 if ($i >= 6) {
  echo '</div><div id="all_news_link"><br /><a href="javascript:all_news()" class="date"><img src="images/reply_down.gif" border="0" alt="" /> Összes hír..</a></div>';
 }
} else {
 echo '<center>Nincs.</center>';
}
?>
</div>
<div class="menu_title">Legújabb kép</div>
<div class="menu_box" align="center">
<?php
$query = mysql_query("SELECT * FROM images WHERE hidden = 0 ORDER BY id DESC LIMIT 1");
$kep = mysql_fetch_array($query);
if ($kep[id] > 0) {
$sql = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$kep[name]'"));
$forumname = $sql[forum_name];
if (strlen($forumname) < 3) { $forumname = $sql[name]; }
echo '
<a href="?a=gallery/image&amp;id='.$kep[id].'" title="Megnyitás nagy méretben" target="_blank">
<img src="images/gallery/'.$kep[id].'_th.jpg" border="0" alt="" />
</a>
<br />
<b><a href="?p=main/userdata&amp;user='.$sql[name].'" class="link">'.text($forumname).'</a></b>
<a href="?p=gallery/gallery&amp;user='.$sql[name].'" title="Képek ettől a felhasználótól"><img src="images/reply_down.gif" border="0" alt="" /></a>
<br />
'.$kep[date].'<br />
';
} else {
 echo 'Nincs kép.';
}
?>
</div>
<div class="menu_title">Online felhasználók</div>
<div class="menu_box">
<?php
$time = date("YmdHi");
$query = mysql_query("SELECT name,forum_name,login2 FROM users WHERE login2 > '".($time-3)."'");
$online_users = mysql_num_rows($query);
if ($online_users > 0) {
 while($sql = mysql_fetch_array($query)) {
  $forumname = $sql[forum_name];
  if (strlen($forumname) < 3) { $forumname = $sql[name]; }
  echo '<img src="images/people.png" alt="" />&nbsp;<b><a href="?p=main/userdata&amp;user='.$sql[name].'" class="link">'.text($forumname).'</a></b><br />'."\n";
 }
} else {
 echo '<center>Nincs.</center>';
}
?>
</div>
</td>
</tr>
<tr>
<td class="copyright" style="background-color: #F0F0F0"><a href="http://validator.w3.org/check?uri=http%3A%2F%2Fflyff.extra.hu%2F" target="_blank"><img src="images/xhtml.jpg" border="0" alt="" /></a></td>
<td class="copyright" style="background-color: #FFFFFF">&copy; GÖRZSÖNY GÁBOR 2007 - 2010</td>
<td class="copyright" style="background-color: #F0F0F0"><a href="http://jigsaw.w3.org/css-validator/validator?uri=http%3A%2F%2Fflyff.extra.hu%2F" target="_blank"><img src="images/css.jpg" border="0" alt="" /></a></td>
</tr>
<tr><td class="shadow_bottom_left"></td><td colspan="3" class="shadow_bottom"></td><td class="shadow_bottom_right"></td></tr>
</table>
<?php
 if ($pm > $_SESSION[alert]) {
  echo '<script language="Javascript" type="text/javascript">alert("'.$pm.' új üzenet érkezett!");</script>';
  if ($_SESSION[alert] == 0) {
   $_SESSION[alert] = $pm;
  }
 }
 if ($_SESSION[alert] > $pm) {
  $_SESSION[alert] = 0;
 }
?>
</center></body>
</html>
<?php } else { ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="hu" lang="hu">
<head>
 <title>Magyar Flyff gamerek oldala</title>
 <meta name="generator" content="handmade" />
 <meta name="Author" content="Görzsöny Gábor" />
 <link rel="StyleSheet" href="style/style.css" type="text/css" />
</head>
<body>
<br /><font style="font-size: 20px"><b>Figyelem!</b></font><br />
Weboldalunk jelenleg az adatbázis hibája miatt nem képes kiszolgálni.<br />
Kérjük, látogass el ide később.<br />
</body>
</html>
<?php } ?>