<div class="title">Regisztr�ci�</div>
<br />
<br />
<?php
if ($_GET[n] == 'terms') {
 $comment = '<div class="red_line" id="notice">Nem fogadtad el a felhaszn�l�si felt�teleket!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badmail') {
 $comment = '<div class="red_line" id="notice">A megadott e-mail c�m nem megfelel�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw') {
 $comment = '<div class="red_line" id="notice">Az egyik jelsz� nem megfelel�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw2') {
 $comment = '<div class="red_line" id="notice">A k�t megadott jelsz� nem egyezik!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'usedname') {
 $comment = '<div class="red_line" id="notice">A megadott n�v m�r haszn�latban van!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badname') {
 $comment = '<div class="red_line" id="notice">A megadott n�v nem megfelel�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
?>
<table width="98%"><tr valign="top"><td width="50%">
<form action="?a=main/register&amp;p=main/reg" method="post">
<b>Felhaszn�l�n�v:</b><br /><input type="text" name="name" id="name" onBlur="test('name',3,20)" style="width: 90%" />
<br /><br />
<b>Jelsz�:</b><br /><input type="password" name="pass_elso" id="pass_elso" onBlur="test('pass_elso',3,20)" style="width: 90%" />
<br /><br />
<b>Jelsz� m�gegyszer:</b><br /><input type="password" name="pass_masodik" id="pass_masodik" onBlur="test_pw('pass_masodik','pass_elso')" style="width: 90%" />
<br /><br />
<b>E-mail c�m:</b><br /><input type="text" name="email" id="email" onBlur="test_mail('email')" style="width: 90%" />
<br /><br />
<table style="width: 100%"><tr valign="top">
<td><input type="checkbox" name="rules" /></td>
<td><b>Elfogadom a felhaszn�l�si felt�teleket *</b></td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
</td><td width="50%" align="justify">
<b><u>Felhaszn�l�si felt�telek</u></b>
<br /><br />
<img src="images/arrow.png" alt="" />
Egy szem�ly csak egyszer regisztr�lhat a weboldalon. Amennyiben ezt nem tartja be, az val�sz�n�leg a regisztr�ci� t�rl�s�t vonja maga ut�n.
<br /><br />
<img src="images/arrow.png" alt="" />
Lehet�s�g szerint k�rj�k a val�s e-mail c�m megad�s�t. Ez fontos lehet �j jelsz� ig�nyl�se eset�n. 
A megadott e-mail c�met m�s felhaszn�l�k sz�m�ra nem adjuk ki, a jelsz�t k�dolt form�ban t�roljuk.
<br /><br />
<img src="images/arrow.png" alt="" />
A felhaszn�l� felel�ss�ggel tartozik az adatai�rt (f�rum n�v, al��r�s, karakter adatok stb) �s �zenetei�rt (f�rum �s priv�t �zenetek), valamint nem k�s�relhet meg a weboldalon b�rmilyen k�rt 
okozni vagy m�sik felhaszn�l�val bejelentkezni.
<br /><br />
<img src="images/arrow.png" alt="" />
Amennyiben egy moder�tor vagy admin sz�ks�gesnek gondolja a regisztr�ci� letilt�s�t vagy t�rl�s�t, ezt megtehetik �s a felhaszn�l�knak nem 
tartoznak elsz�mol�ssal.
</td></tr></table>
<br />
* A felhaszn�l�si felt�telek elfogad�s�val tudom�sul veszem, hogy a felhaszn�l�si felt�telek megv�ltoz�s�val azok tov�bbra is �rv�nyesek maradnak a felhaszn�l�ra.
<br /><br />
Ha b�rmilyen k�rd�sed van, vedd fel a kapcsolatot <a href="?p=main/contact">vel�nk</a>.<br /><br />
</form>
