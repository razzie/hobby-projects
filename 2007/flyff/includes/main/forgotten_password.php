<div class="title">Elfelejtett jelszó</div>
<?php
if ($_GET[n] == 'ok') {
 $comment = '<div class="green_line" id="notice">Az e-mail sikeresen elküldve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">Az e-mail küldése nem sikerült!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'wrong') {
 $comment = '<div class="red_line" id="notice">Nincs ilyen felhasználó!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo '<br />'.$comment;
?>
<p align="justify">
Írd be a felhasználóneved (bejelentkezési név) és amennyiben helyes, a rendszer elküldi a megadott e-mail címedre
 az új generált jelszavad és egy aktiváló linket. Ha nem valós email címet adtál meg, akkor vedd fel
 <a href="?p=main/contact">velünk</a> a kapcsolatot vagy <a href="?p=main/reg">regisztrálj</a> egy új felhasználónevet!
<br /><br />
<form action="?a=main/send_password" method="POST">
<input type="text" name="name" style="width: 150px" />
<input type="submit" class="button" value="Küldés" />
</from></p>
