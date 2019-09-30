<?php
$hiba = "false";
$new_pass = rand(0,9).rand(0,9).rand(0,9).rand(0,9);
$vmi = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_POST[name]' LIMIT 1"));
$mail = $vmi[email];
if ($vmi[name] <> $_POST[name]) { $hiba = 'true'; }
if ($hiba == "true") {
 $comment = 'wrong';
} else {
$name = 'From: '.$_POST[name]."\n";
$uzenet = 'Kérés érkezett a rendszerbe '.$_POST[name].' nevû felhasználó jelszavának módosítására.
Amennyiben ezt az üzenetet más küldte, semmit sem kell tenned.

Felhasználói név: '.$_POST[name].'
Az új generált jelszó: '.$new_pass.'

A jelszó megváltoztatásához kattints a következõ linkre!
(Ha nem mûködik a link, akkor másold be azt a böngészõ címsorába!)
http://flyff.extra.hu/?a=main/set_pw&p=login/index&name='.$_POST[name].'&pw='.$new_pass.'&old_pw='.$vmi[pass].'

Ez egy automatikusan generált üzenet, ne válaszolj rá!
http://flyff.extra.hu?p=login/index'."\n";
if ($vmi[name] == $_POST[name]) {
 if ($hiba == "false") { $kuldes = mail($mail, "Elfelejtett jelszó", $uzenet, $name); }
 if (!$kuldes) { $hiba = "true"; }
 if ($hiba == "true") {
  $comment = 'fail';
 } else {
  $comment = 'ok';
 }
}
header('Location: ?p=main/forgotten_password&n='.$comment);
}
?>