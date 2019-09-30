<?php
if ($_SESSION[rank] > 0 && !$_SESSION[name]) { $_SESSION[rank] = ""; }
if (!$_SESSION[name] && $_COOKIE[name] && $_COOKIE[password]) {
 $vmi = mysql_fetch_array(mysql_query("SELECT name,pass,rank,banned FROM users WHERE name = '$_COOKIE[name]' LIMIT 1"));
 if ($_COOKIE[password] == $vmi[pass] && $_COOKIE[name] == $vmi[name] && $vmi[banned] == 0) {
  $_SESSION[name] = $_COOKIE[name];
  $_SESSION[pass] = $_COOKIE[password];
  $_SESSION[rank] = $vmi[rank];
 }
}
if ($_SESSION["name"]) {
$vmi = mysql_fetch_array(mysql_query("SELECT name,pass,rank,banned FROM users WHERE name = '".$_SESSION[name]."' LIMIT 1"));
if ($vmi[pass] == $_SESSION[pass] && $vmi[name] == $_SESSION[name] && $vmi[banned] == 0) {
 $time = date("YmdHi");
 $ip = $_SERVER[REMOTE_ADDR];
 $_SESSION[rank] = $vmi[rank];
 mysql_query("UPDATE users SET ip = '$ip', login = NOW(), login2 = '$time' WHERE name = '$_SESSION[name]' LIMIT 1");
 if ($vmi[rank] < 1) {
  $query = mysql_fetch_array(mysql_query("SELECT * FROM ip_list WHERE ip = '$ip' LIMIT 1"));
  if ($sql[ip] == $ip) {
   $_SESSION["name"] = "";
   $_SESSION["pass"] = "";
   $_SESSION["rank"] = "";
  }
 }
} else {
 $_SESSION["name"] = "";
 $_SESSION["pass"] = "";
 $_SESSION["rank"] = "";
}
}
function images($valtozo) {
 if (substr_count($valtozo, '#image_')) {
  $valtozo = str_replace("\n", ' <br /> ', $valtozo);
  $tomb = explode(' ', $valtozo);
  $valtozo = "";
  for ($i=0; $i<=count($tomb); $i++)
  {
   if (substr($tomb[$i], 0, 7) == "#image_")
   {
    $image = substr($tomb[$i], 7, strlen($tomb[$i])-7);
    $sql = mysql_num_rows(mysql_query("SELECT id FROM images WHERE id = '$image'"));
    if ($sql > 0) {
     $tomb[$i] = '<a href="?a=gallery/image&amp;id='.$image.'" target="_blank" title="Megnyitás nagy méretben"><img src="images/gallery/'.$image.'_th.jpg" border="0" alt="" /></a>';
    } else {
     $tomb[$i] = '<img src="images/delete.png" alt="" /> <b>Nem létezõ kép. ('.$image.')</b>';
    }
   }
   $valtozo .= $tomb[$i].' ';
  }
 }
 if (substr_count($valtozo, '#quote') == substr_count($valtozo, '#/quote') && strpos($valtozo, '#/quote') > strpos($valtozo, '#quote')) {
  $valtozo = str_replace('#quote', '<center><div class="quote_box">', $valtozo);
  $valtozo = str_replace('#/quote', '</div></center>', $valtozo);
 } else {
  $valtozo = str_replace('#quote', '', $valtozo);
  $valtozo = str_replace('#/quote', '', $valtozo);
 }
 return $valtozo;
}
function text($valtozo) {
 $valtozo = str_replace("\n", ' <br /> ', $valtozo);
 $tomb = explode(' ', $valtozo);
 $valtozo = "";
 for ($i=0; $i<=count($tomb); $i++)
 {
  if (substr($tomb[$i], 0, 4) == "www.")
  {
   $link = $tomb[$i];
   $link = str_replace('&amp;', '&', $link);
   if (strlen($link) > 30) {
    $short_link = substr($link, 0, 28).'..';
   } else {
    $short_link = $link;
   }
   $tomb[$i] = '<a href="http://'.$link.'" title="'.$link.'" target="_blank">'.$short_link.'</a>';
  }
  if (substr($tomb[$i], 0, 5) == "www2.")
  {
   $link = $tomb[$i];
   $link = str_replace('&amp;', '&', $link);
   if (strlen($link) > 30) {
    $short_link = substr($link, 0, 30).'..';
   } else {
    $short_link = $link;
   }
   $tomb[$i] = '<a href="http://'.$link.'" title="'.$link.'" target="_blank">'.$short_link.'</a>';
  }
  if (substr($tomb[$i], 0, 7) == "http://")
  {
   $link = $tomb[$i];
   $link = str_replace('&amp;', '&', $link);
   $short_link = substr($link, 7, strlen($link)-7);
   if (strlen($short_link) > 30) {
    $short_link = substr($short_link, 0, 30).'..';
   }
   $tomb[$i] = '<a href="'.$link.'" title="'.$link.'" target="_blank">'.$short_link.'</a>';
  }
  $valtozo .= $tomb[$i].' ';
 }
 /* Szín formázások */
 if (substr_count($valtozo, '#red') == substr_count($valtozo, '#/red') && strpos($valtozo, '#/red') > strpos($valtozo, '#red')) {
  $valtozo = str_replace('#red', '<font color="#FF0000">', $valtozo);
  $valtozo = str_replace('#/red', '</font>', $valtozo);
 } else {
  $valtozo = str_replace('#red', '', $valtozo);
  $valtozo = str_replace('#/red', '', $valtozo);
 }
 if (substr_count($valtozo, '#green') == substr_count($valtozo, '#/green') && strpos($valtozo, '#/green') > strpos($valtozo, '#green')) {
  $valtozo = str_replace('#green', '<font color="#00DD00">', $valtozo);
  $valtozo = str_replace('#/green', '</font>', $valtozo);
 } else {
  $valtozo = str_replace('#green', '', $valtozo);
  $valtozo = str_replace('#/green', '', $valtozo);
 }
 if (substr_count($valtozo, '#blue') == substr_count($valtozo, '#/blue') && strpos($valtozo, '#/blue') > strpos($valtozo, '#blue')) {
  $valtozo = str_replace('#blue', '<font color="#0000FF">', $valtozo);
  $valtozo = str_replace('#/blue', '</font>', $valtozo);
 } else {
  $valtozo = str_replace('#blue', '', $valtozo);
  $valtozo = str_replace('#/blue', '', $valtozo);
 }
 if (substr_count($valtozo, '#yellow') == substr_count($valtozo, '#/yellow') && strpos($valtozo, '#/yellow') > strpos($valtozo, '#yellow')) {
  $valtozo = str_replace('#yellow', '<font color="#DDDD00">', $valtozo);
  $valtozo = str_replace('#/yellow', '</font>', $valtozo);
 } else {
  $valtozo = str_replace('#yellow', '', $valtozo);
  $valtozo = str_replace('#/yellow', '', $valtozo);
 }
 if (substr_count($valtozo, '#grey') == substr_count($valtozo, '#/grey') && strpos($valtozo, '#/grey') > strpos($valtozo, '#grey')) {
  $valtozo = str_replace('#grey', '<font color="#666666">', $valtozo);
  $valtozo = str_replace('#/grey', '</font>', $valtozo);
 } else {
  $valtozo = str_replace('#grey', '', $valtozo);
  $valtozo = str_replace('#/grey', '', $valtozo);
 }

 /* Formázások */
 if (substr_count($valtozo, '#b') == substr_count($valtozo, '#/b') && strpos($valtozo, '#/b') > strpos($valtozo, '#b')) {
  $valtozo = str_replace('#b', '<b>', $valtozo);
  $valtozo = str_replace('#/b', '</b>', $valtozo);
 } else {
  $valtozo = str_replace('#b', '', $valtozo);
  $valtozo = str_replace('#/b', '', $valtozo);
 }
 if (substr_count($valtozo, '#i') == substr_count($valtozo, '#/i') && strpos($valtozo, '#/i') > strpos($valtozo, '#i')) {
  $valtozo = str_replace('#i', '<i>', $valtozo);
  $valtozo = str_replace('#/i', '</i>', $valtozo);
 } else {
  $valtozo = str_replace('#i', '', $valtozo);
  $valtozo = str_replace('#/i', '', $valtozo);
 }
 if (substr_count($valtozo, '#u') == substr_count($valtozo, '#/u') && strpos($valtozo, '#/u') > strpos($valtozo, '#u')) {
  $valtozo = str_replace('#u', '<u>', $valtozo);
  $valtozo = str_replace('#/u', '</u>', $valtozo);
 } else {
  $valtozo = str_replace('#u', '', $valtozo);
  $valtozo = str_replace('#/u', '', $valtozo);
 }
 return $valtozo;
}
function no_format($valtozo) {
 /* Szín formázások */
  $valtozo = str_replace('#red', '', $valtozo);
  $valtozo = str_replace('#/red', '', $valtozo);
  $valtozo = str_replace('#green', '', $valtozo);
  $valtozo = str_replace('#/green', '', $valtozo);
  $valtozo = str_replace('#blue', '', $valtozo);
  $valtozo = str_replace('#/blue', '', $valtozo);
  $valtozo = str_replace('#yellow', '', $valtozo);
  $valtozo = str_replace('#/yellow', '', $valtozo);
  $valtozo = str_replace('#grey', '', $valtozo);
  $valtozo = str_replace('#/grey', '', $valtozo);

 /* Egyéb formázások */
  $valtozo = str_replace('#b', '', $valtozo);
  $valtozo = str_replace('#/b', '', $valtozo);
  $valtozo = str_replace('#i', '', $valtozo);
  $valtozo = str_replace('#/i', '', $valtozo);
  $valtozo = str_replace('#u', '', $valtozo);
  $valtozo = str_replace('#/u', '', $valtozo);

 return $valtozo;
}
function smile($valtozo) {
 $valtozo = str_replace(':)', '<img src="images/smiley/smile.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':D', '<img src="images/smiley/biggrin.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':lol', '<img src="images/smiley/laugh.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace('xD', '<img src="images/smiley/xd.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace('^^', '<img src="images/smiley/happy.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(';)', '<img src="images/smiley/wink.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':P', '<img src="images/smiley/tongue.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace('B)', '<img src="images/smiley/cool.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':??', '<img src="images/smiley/huh.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':o', '<img src="images/smiley/ohmy.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':(', '<img src="images/smiley/sad.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace('-.-', '<img src="images/smiley/dry.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':@', '<img src="images/smiley/mad.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':rolleyes', '<img src="images/smiley/rolleyes.gif" border="0" alt="" />', $valtozo);
 $valtozo = str_replace(':blush', '<img src="images/smiley/blush.gif" border="0" alt="" />', $valtozo);
 return $valtozo;
}
function test($valtozo,$min,$max) {
 $error = 1;
 $valtozo_no_space = str_replace(' ', '', $valtozo);

 if (strlen($valtozo_no_space) < $min) { $error = 0; }
 if (strlen($valtozo) > $max) { $error = 0; }

 if (substr_count($valtozo, "\n") > 0) { $error = 0; }

 return $error;
}
function test2($valtozo,$min,$max) {
 $error = 1;
 $valtozo_no_space = str_replace(' ', '', $valtozo);

 if (strlen($valtozo_no_space) < $min) { $error = 0; }
 if (strlen($valtozo) > $max) { $error = 0; }

 return $error;
}
function test3($valtozo,$min,$max) {
 $error = 1;
 $valtozo_no_space = str_replace(' ', '', $valtozo);

 if (strlen($valtozo_no_space) < $min) { $error = 0; }
 if (strlen($valtozo) > $max) { $error = 0; }

 if (substr_count($valtozo, "\n") > 0) { $error = 0; }
 if (substr_count($valtozo, "&") > 0) { $error = 0; }
 if (substr_count($valtozo, ">") > 0) { $error = 0; }
 if (substr_count($valtozo, "<") > 0) { $error = 0; }
 if (substr_count($valtozo, "'") > 0) { $error = 0; }
 if (substr_count($valtozo, '"') > 0) { $error = 0; }

 return $error;
}
function formed_time($mode = 0) {
 $year = date("Y");
 $month = date("m");
 $day = date("d");
 $hour = date("H");
 $min = date("i");
 /* Hónap formázása */
 if ($month == 1) { $month = 'január'; }
 if ($month == 2) { $month = 'február'; }
 if ($month == 3) { $month = 'március'; }
 if ($month == 4) { $month = 'április'; }
 if ($month == 5) { $month = 'május'; }
 if ($month == 6) { $month = 'június'; }
 if ($month == 7) { $month = 'július'; }
 if ($month == 8) { $month = 'augusztus'; }
 if ($month == 9) { $month = 'szeptember'; }
 if ($month == 10) { $month = 'október'; }
 if ($month == 11) { $month = 'november'; }
 if ($month == 12) { $month = 'december'; }
 /* Formázott idõ/dátum */
 if ($mode == 0) { $time = $year.". ".$month." ".$day.". - ".$hour." óra ".$min." perc"; }
 if ($mode == 1) { $time = $year.". ".$month." ".$day."."; }
 if ($mode == 2) { $time = $hour." óra ".$min." perc"; }
 return $time;
}
?>