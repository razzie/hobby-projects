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
$uzenet = 'K�r�s �rkezett a rendszerbe '.$_POST[name].' nev� felhaszn�l� jelszav�nak m�dos�t�s�ra.
Amennyiben ezt az �zenetet m�s k�ldte, semmit sem kell tenned.

Felhaszn�l�i n�v: '.$_POST[name].'
Az �j gener�lt jelsz�: '.$new_pass.'

A jelsz� megv�ltoztat�s�hoz kattints a k�vetkez� linkre!
(Ha nem m�k�dik a link, akkor m�sold be azt a b�ng�sz� c�msor�ba!)
http://flyff.extra.hu/?a=main/set_pw&p=login/index&name='.$_POST[name].'&pw='.$new_pass.'&old_pw='.$vmi[pass].'

Ez egy automatikusan gener�lt �zenet, ne v�laszolj r�!
http://flyff.extra.hu?p=login/index'."\n";
if ($vmi[name] == $_POST[name]) {
 if ($hiba == "false") { $kuldes = mail($mail, "Elfelejtett jelsz�", $uzenet, $name); }
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