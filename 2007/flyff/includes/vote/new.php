<?php
if ($_SESSION[rank] > 0) {
?>
<div class="title">Új szavazás</div>
<br />
<br />
<?php
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">A szavazást nem sikerült létrehozni!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
?>
<form action="?a=vote/new_vote&amp;p=vote/new" method="post">
<table><tr>
<td width="100px"><b>Szavazás címe:</b><br /><br /></td><td><input type="text" id="title" name="title" style="width: 300px" onBlur="test('title',10,300)" /><br /><br /></td>
</tr><tr>
<td width="100px"><b>1. válasz:</b></td><td><input type="text" id="answer1" name="answer1" style="width: 300px" onBlur="test('answer1',3,100)" /></td>
</tr><tr>
<td width="100px"><b>2. válasz:</b></td><td><input type="text" id="answer2" name="answer2" style="width: 300px" onBlur="test('answer2',3,100)" /></td>
</tr><tr>
<td width="100px"><b>3. válasz:</b></td><td><input type="text" id="answer3" name="answer3" style="width: 300px" onBlur="test('answer3',3,100)" /></td>
</tr><tr>
<td width="100px"><b>4. válasz:</b></td><td><input type="text" id="answer4" name="answer4" style="width: 300px" onBlur="test('answer4',3,100)" /></td>
</tr><tr>
<td width="100px"><b>5. válasz:</b></td><td><input type="text" id="answer5" name="answer5" style="width: 300px" onBlur="test('answer5',3,100)" /></td>
</tr><tr>
<td width="100px"><b>6. válasz:</b></td><td><input type="text" id="answer6" name="answer6" style="width: 300px" onBlur="test('answer6',3,100)" /></td>
</tr><tr>
<td width="100px"><b>7. válasz:</b></td><td><input type="text" id="answer7" name="answer7" style="width: 300px" onBlur="test('answer7',3,100)" /></td>
</tr><tr>
<td width="100px"><b>8. válasz:</b></td><td><input type="text" id="answer8" name="answer8" style="width: 300px" onBlur="test('answer8',3,100)" /></td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
<input type="reset" class="button" value="Töröl" />
<input type="button" class="button" onClick="history.back(1)" value="Vissza" />
</form>
<br />
Nem kötelezõ minden válaszmezõ kitöltése, de legalább kettõé igen!
<?php
} else {
 header('Location: ?p=vote/votes');
}
?>
