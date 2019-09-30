<div class="title">Keresés</div><br />
<?php
$key = $_POST['search'];
if ($key) {
 if (test($key,3,50) == 0) {
  echo '<br /><div class="red_line">A megadott kulcsszó nem megfelelõ!</div><br />';
 }
}
?>
<p align="justify">
A keresõ a beírt szót / kifejezést keresi a weboldalon található fórumüzenetek, hírek és a szavazások, valamint a felhasználók és karaktereik között. 
A rendszer kilistázza a találatokra mutató linkeket, a hozzájuk tartozó idõpontokat és egy idézetet.
<br /><br />
<form action="?p=search/search" method="POST">
<b>Kulcsszó:</b><br /><br />
<input type="text" name="search" id="search" onBlur="test('search',3,50)" style="width: 150px" />
<input type="submit" class="button" value="Keresés" />
</from></p>
<br /><br /><br />
