<?php
 $sql = mysql_fetch_array(mysql_query("SELECT * FROM forum WHERE id = ".$_GET[id]." LIMIT 1"));
echo '
<script language="Javascript" type="text/javascript">
function insertSmiley(smiley)
{
  var msg = document.getElementById("message") ;  
  msg.value += smiley ;
}
var date = "'.formed_time(1).'" ;
var time = "'.formed_time(2).'" ;
</script>
';
?>
<div class="title">Üzenet szerkesztése</div>
<br />
<br />
<?php
echo '<form action="?a=forum/do_edit&amp;p=forum/topic&amp;theme='.$_GET[theme].'&amp;id='.$_GET[id].'&amp;page='.$_GET[page].'" method="POST">';
?>
<table width="100%"><tr valign="top">
<td width="30px"></td>
<td width="530px">
<a onClick="insertSmiley(':)')"><img src="images/smiley/smile.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':D')"><img src="images/smiley/biggrin.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':lol')"><img src="images/smiley/laugh.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('xD')"><img src="images/smiley/xd.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('^^')"><img src="images/smiley/happy.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(';)')"><img src="images/smiley/wink.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':P')"><img src="images/smiley/tongue.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('B)')"><img src="images/smiley/cool.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':??')"><img src="images/smiley/huh.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':o')"><img src="images/smiley/ohmy.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':(')"><img src="images/smiley/sad.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('-.-')"><img src="images/smiley/dry.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':@')"><img src="images/smiley/mad.gif" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(':rolleyes')"><img src="images/smiley/rolleyes.gif" alt="" border="0" /></a>&nbsp; 
<a onClick="insertSmiley(':blush')"><img src="images/smiley/blush.gif" alt="" border="0" /></a>&nbsp; 
</td>
</tr><tr valign="top">
<td width="30px">
<a onClick="insertSmiley(' '+date)"><img src="images/date.png" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley(' '+time)"><img src="images/time.png" border="0" alt="" /></a>&nbsp; 
<br /><br />
<a onClick="insertSmiley('#b #/b')"><img src="images/bold.png" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('#i #/i')"><img src="images/italic.png" border="0" alt="" /></a>&nbsp; 
<a onClick="insertSmiley('#u #/u')"><img src="images/underlined.png" border="0" alt="" /></a>&nbsp;
</td>
<td width="530px">
<textarea name="message" id="message" onBlur="test('message',3,2000)" style="width: 100%; height: 300px">
<?php echo $sql[message]; ?>
</textarea>
</td>
</tr></table>
<br />
<input type="submit" class="button" value="Rendben" />
<input type="reset" class="button" value="Újra" />
<input type="button" class="button" onClick="history.back(1)" value="Vissza" />
</form>
<br />
