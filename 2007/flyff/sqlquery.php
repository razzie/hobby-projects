<?php

$query = $_POST['querytext'];
$err = false;

$nt1=false;
$str1="'";
if(strpos($query,"'")!==false)
{
	$str1='"';
	if(strpos($query,'"')!==false)
	{
		$str1="";
		$nt1=true;
	}
}

echo "<HTML><HEAD><TITLE>MiniMysqlQueryTool</TITLE>";
echo '<script language="JavaScript">';
if(!$nt1)echo 'var querytext='.$str1.$query.$str1.';';
else echo 'var querytext="";';
echo 'function resetpasswd() {document.all.passwdid.value="";}';
echo 'function load() {document.all.querytextid.setActive();setTimeout("resetpasswd()", 1000*64*10);}';
echo 'function writeqback() {document.all.querytextid.value=querytext;document.all.querytextid.setActive();}';
echo '</script></HEAD><BODY onload="load()">';

$host = null;
$user= "";
$passwd="";
$databasename="";
$shownulls=false;
if(($_POST["host"]!=null)&&($_POST["host"]!=""))$host=$_POST["host"];
if($_POST["user"]!=null)$user=$_POST["user"];
if($_POST["passwd"]!=null)$passwd=$_POST["passwd"];
if($_POST["databasename"]!=null)$databasename=$_POST["databasename"];
if($_POST["shownulls"]!=null)$shownulls=$_POST["shownulls"];

if($host!=null)if(!$link = mysql_connect($host, $user, $passwd))echo "Error on connect: ". mysql_error()."<BR>";
else
{
	$trimquery=trim($query);
	if(strtolower(substr($trimquery, 0, 3))=="use")
	{
		$query=str_replace(";","",$query);
		$trimquery=trim($query);
		$str1=trim(substr($trimquery, 3));
		if(mysql_select_db($str1))$databasename=$str1;
	}else mysql_select_db($databasename);

	if($query!=null)
	{
		echo "Query=".$query.'<BR>';
		$result = mysql_query($query);
		if(($result!=false)&&($result!=0)&&($result!=null))
		{
			$b=0;
			if(is_resource($result))$b=mysql_num_fields($result);
			if(($b>0)&&(is_resource($result)))
			{
				echo "<table border='1'><tr>";
				for($a=0;$a<$b;$a++)echo "<td>".mysql_field_name($result, $a)."</td>";
				echo "</tr>";
				while($row = mysql_fetch_array($result))
				{
					echo "<tr>";
					for($a=0;$a<$b;$a++)
					{
						$str1=$row[$a];
						if(($shownulls)&&($str1==null))$str1="null";
						echo "<td>".$str1."</td>";

					}
					echo "</tr>";
				}
				echo "</table>";
				if(is_resource($result))mysql_free_result($result);
			}

			$str1=strtolower(substr($trimquery, 0, 6));
			if($str1=="insert")echo "Added ";
			if($str1=="delete")echo "Deleted ";
			echo mysql_affected_rows($link)." rows<br>";


		}else 
		{
			echo mysql_error()."<BR>";
			$err=true;
		}
	}
	mysql_close($link);
}

if(($query!=null)&&($query!="")&&(!$nt1))echo '<input type="button" value="Again" onclick="writeqback()">';
echo '<FORM ACTION="sqlquery.php" METHOD=POST><table border=0>';

echo '<tr><td>Query:		</td><td><INPUT type=text id="querytextid" NAME=querytext size=100 '.(($err)?('value="'.$query.'"'):'').'></td></tr>';
echo '<tr><td>Host:		</td><td><INPUT type=text NAME=host';if($host!=null)echo ' value="'.$host.'"';echo '></td></tr>';
echo '<tr><td>User:		</td><td><INPUT type=text NAME=user';if($user!="")echo ' value="'.$user.'"';echo '></td></tr>';
echo '<tr><td>Password:		</td><td><INPUT type=password id="passwdid" NAME=passwd';if($passwd!="")echo ' value="'.$passwd.'"';echo '></td></tr>';
echo '<tr><td>Database:		</td><td><INPUT type=text NAME=databasename';if($databasename!="")echo ' value="'.$databasename.'"';echo '></td></tr>';
echo '<tr><td>Write Nulls:	</td><td><INPUT type=checkbox NAME=shownulls value="true" '.(($shownulls)?'checked':'').'></td></tr></table>';

echo '<INPUT TYPE=submit>';

if($nt1)echo "<BR>Please use only one of the ".'"'." ' characters";

echo '</BODY></HTML>';
?>