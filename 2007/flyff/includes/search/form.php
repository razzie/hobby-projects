<div class="title">Keres�s</div><br />
<?php
$key = $_POST['search'];
if ($key) {
 if (test($key,3,50) == 0) {
  echo '<br /><div class="red_line">A megadott kulcssz� nem megfelel�!</div><br />';
 }
}
?>
<p align="justify">
A keres� a be�rt sz�t / kifejez�st keresi a weboldalon tal�lhat� f�rum�zenetek, h�rek �s a szavaz�sok, valamint a felhaszn�l�k �s karaktereik k�z�tt. 
A rendszer kilist�zza a tal�latokra mutat� linkeket, a hozz�juk tartoz� id�pontokat �s egy id�zetet.
<br /><br />
<form action="?p=search/search" method="POST">
<b>Kulcssz�:</b><br /><br />
<input type="text" name="search" id="search" onBlur="test('search',3,50)" style="width: 150px" />
<input type="submit" class="button" value="Keres�s" />
</from></p>
<br /><br /><br />
