<div class="title">Elfelejtett jelsz�</div>
<?php
if ($_GET[n] == 'ok') {
 $comment = '<div class="green_line" id="notice">Az e-mail sikeresen elk�ldve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">Az e-mail k�ld�se nem siker�lt!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'wrong') {
 $comment = '<div class="red_line" id="notice">Nincs ilyen felhaszn�l�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo '<br />'.$comment;
?>
<p align="justify">
�rd be a felhaszn�l�neved (bejelentkez�si n�v) �s amennyiben helyes, a rendszer elk�ldi a megadott e-mail c�medre
 az �j gener�lt jelszavad �s egy aktiv�l� linket. Ha nem val�s email c�met adt�l meg, akkor vedd fel
 <a href="?p=main/contact">vel�nk</a> a kapcsolatot vagy <a href="?p=main/reg">regisztr�lj</a> egy �j felhaszn�l�nevet!
<br /><br />
<form action="?a=main/send_password" method="POST">
<input type="text" name="name" style="width: 150px" />
<input type="submit" class="button" value="K�ld�s" />
</from></p>
