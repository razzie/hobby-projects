<div class="title">Regisztráció</div>
<br />
<br />
<?php
if ($_GET[n] == 'terms') {
 $comment = '<div class="red_line" id="notice">Nem fogadtad el a felhasználási feltételeket!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badmail') {
 $comment = '<div class="red_line" id="notice">A megadott e-mail cím nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw') {
 $comment = '<div class="red_line" id="notice">Az egyik jelszó nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw2') {
 $comment = '<div class="red_line" id="notice">A két megadott jelszó nem egyezik!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'usedname') {
 $comment = '<div class="red_line" id="notice">A megadott név már használatban van!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badname') {
 $comment = '<div class="red_line" id="notice">A megadott név nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
?>
<table width="98%"><tr valign="top"><td width="50%">
<form action="?a=main/register&amp;p=main/reg" method="post">
<b>Felhasználónév:</b><br /><input type="text" name="name" id="name" onBlur="test('name',3,20)" style="width: 90%" />
<br /><br />
<b>Jelszó:</b><br /><input type="password" name="pass_elso" id="pass_elso" onBlur="test('pass_elso',3,20)" style="width: 90%" />
<br /><br />
<b>Jelszó mégegyszer:</b><br /><input type="password" name="pass_masodik" id="pass_masodik" onBlur="test_pw('pass_masodik','pass_elso')" style="width: 90%" />
<br /><br />
<b>E-mail cím:</b><br /><input type="text" name="email" id="email" onBlur="test_mail('email')" style="width: 90%" />
<br /><br />
<table style="width: 100%"><tr valign="top">
<td><input type="checkbox" name="rules" /></td>
<td><b>Elfogadom a felhasználási feltételeket *</b></td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
</td><td width="50%" align="justify">
<b><u>Felhasználási feltételek</u></b>
<br /><br />
<img src="images/arrow.png" alt="" />
Egy személy csak egyszer regisztrálhat a weboldalon. Amennyiben ezt nem tartja be, az valószínûleg a regisztráció törlését vonja maga után.
<br /><br />
<img src="images/arrow.png" alt="" />
Lehetõség szerint kérjük a valós e-mail cím megadását. Ez fontos lehet új jelszó igénylése esetén. 
A megadott e-mail címet más felhasználók számára nem adjuk ki, a jelszót kódolt formában tároljuk.
<br /><br />
<img src="images/arrow.png" alt="" />
A felhasználó felelõsséggel tartozik az adataiért (fórum név, aláírás, karakter adatok stb) és üzeneteiért (fórum és privát üzenetek), valamint nem kísérelhet meg a weboldalon bármilyen kárt 
okozni vagy másik felhasználóval bejelentkezni.
<br /><br />
<img src="images/arrow.png" alt="" />
Amennyiben egy moderátor vagy admin szükségesnek gondolja a regisztráció letiltását vagy törlését, ezt megtehetik és a felhasználóknak nem 
tartoznak elszámolással.
</td></tr></table>
<br />
* A felhasználási feltételek elfogadásával tudomásul veszem, hogy a felhasználási feltételek megváltozásával azok továbbra is érvényesek maradnak a felhasználóra.
<br /><br />
Ha bármilyen kérdésed van, vedd fel a kapcsolatot <a href="?p=main/contact">velünk</a>.<br /><br />
</form>
