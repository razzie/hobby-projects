<div class="title">Bejelentkez�s</div>
<br /><br />
<?php
if ($_GET[n] == 'ok') {
 $comment = '<div class="green_line" id="notice">A regisztr�ci� sikeres volt, mostm�r bejelentkezhetsz!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'wrong') {
 $comment = '<div class="red_line" id="notice">Hib�s felhaszn�l�n�v �s/vagy jelsz�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'ban') {
 $comment = '<div class="red_line" id="notice">Ez a felhaszn�l�n�v le van tiltva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'ip_ban') {
 $comment = '<div class="red_line" id="notice">Ez az IP c�m le van tiltva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
?>
<form action="?a=login/login&amp;p=main/home" method="POST">
<b>Felhaszn�l�n�v:</b><br />
<input type="text" name="name" style="width: 50%" /><br /><br />
<b>Jelsz�:</b><br />
<input type="password" name="pass" style="width: 50%" /><br /><br />
<input type="checkbox" name="cookie" checked /><b> Automatikus bejelentkez�s</b><br /><br />
<input type="submit" class="button" value="Rendben" /><br /><br />
Ha m�g nem vagy regisztr�lva, <a href="?p=main/reg" class="link"><b>itt</b></a> megteheted.<br />
Elfelejtetted a jelszavad? Kattints <a href="?p=main/forgotten_password" title="Elfelejtett jelsz�" class="link"><b>ide</b></a>.
</form>
