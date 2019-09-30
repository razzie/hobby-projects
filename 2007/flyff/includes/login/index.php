<div class="title">Bejelentkezés</div>
<br /><br />
<?php
if ($_GET[n] == 'ok') {
 $comment = '<div class="green_line" id="notice">A regisztráció sikeres volt, mostmár bejelentkezhetsz!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'wrong') {
 $comment = '<div class="red_line" id="notice">Hibás felhasználónév és/vagy jelszó!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'ban') {
 $comment = '<div class="red_line" id="notice">Ez a felhasználónév le van tiltva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'ip_ban') {
 $comment = '<div class="red_line" id="notice">Ez az IP cím le van tiltva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
?>
<form action="?a=login/login&amp;p=main/home" method="POST">
<b>Felhasználónév:</b><br />
<input type="text" name="name" style="width: 50%" /><br /><br />
<b>Jelszó:</b><br />
<input type="password" name="pass" style="width: 50%" /><br /><br />
<input type="checkbox" name="cookie" checked /><b> Automatikus bejelentkezés</b><br /><br />
<input type="submit" class="button" value="Rendben" /><br /><br />
Ha még nem vagy regisztrálva, <a href="?p=main/reg" class="link"><b>itt</b></a> megteheted.<br />
Elfelejtetted a jelszavad? Kattints <a href="?p=main/forgotten_password" title="Elfelejtett jelszó" class="link"><b>ide</b></a>.
</form>
