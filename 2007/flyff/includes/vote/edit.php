<?php
if ($_SESSION[rank] > 0 && $_GET[id]) {
?>
<div class="title">Szerkeszt�s</div>
<br />
<br />
<?php
$sql = mysql_fetch_array(mysql_query("SELECT * FROM votes WHERE id = '$_GET[id]'"));
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">A v�ltoztat�s sikertelen volt!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
echo '<form action="?a=vote/edit_vote&amp;id='.$_GET[id].'" method="post">';
?>
<table><tr>
<td width="100px"><b>Szavaz�s c�me:</b><br /><br /></td><td><input type="text" id="title" name="title" style="width: 300px" onBlur="test('title',10,300)" value="<?php echo $sql[title]; ?>" /><br /><br /></td>
</tr><tr>
<td width="100px"><b>1. v�lasz:</b></td><td><input type="text" id="answer1" name="answer1" style="width: 300px" onBlur="test('answer1',3,100)" value="<?php echo $sql[answer1]; ?>" /></td>
</tr><tr>
<td width="100px"><b>2. v�lasz:</b></td><td><input type="text" id="answer2" name="answer2" style="width: 300px" onBlur="test('answer2',3,100)" value="<?php echo $sql[answer2]; ?>" /></td>
</tr><tr>
<td width="100px"><b>3. v�lasz:</b></td><td><input type="text" id="answer3" name="answer3" style="width: 300px" onBlur="test('answer3',3,100)" value="<?php echo $sql[answer3]; ?>" /></td>
</tr><tr>
<td width="100px"><b>4. v�lasz:</b></td><td><input type="text" id="answer4" name="answer4" style="width: 300px" onBlur="test('answer4',3,100)" value="<?php echo $sql[answer4]; ?>" /></td>
</tr><tr>
<td width="100px"><b>5. v�lasz:</b></td><td><input type="text" id="answer5" name="answer5" style="width: 300px" onBlur="test('answer5',3,100)" value="<?php echo $sql[answer5]; ?>" /></td>
</tr><tr>
<td width="100px"><b>6. v�lasz:</b></td><td><input type="text" id="answer6" name="answer6" style="width: 300px" onBlur="test('answer6',3,100)" value="<?php echo $sql[answer6]; ?>" /></td>
</tr><tr>
<td width="100px"><b>7. v�lasz:</b></td><td><input type="text" id="answer7" name="answer7" style="width: 300px" onBlur="test('answer7',3,100)" value="<?php echo $sql[answer7]; ?>" /></td>
</tr><tr>
<td width="100px"><b>8. v�lasz:</b></td><td><input type="text" id="answer8" name="answer8" style="width: 300px" onBlur="test('answer8',3,100)" value="<?php echo $sql[answer8]; ?>" /></td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
<input type="reset" class="button" value="T�r�l" />
<input type="button" class="button" onClick="history.back(1)" value="Vissza" />
</form>
<br />
Nem k�telez� minden v�laszmez� kit�lt�se, de legal�bb kett�� igen!
<?php
} else {
 header('Location: ?p=vote/votes&id='.$_GET[id]);
}
?>