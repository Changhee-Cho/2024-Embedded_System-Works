<html>
<title> [ PHP & Raspberry Pi ] </title>
<body>
<script language='javascript'>
window.setTimeout('window.location.reload()', 1000);
</script>
<h3> PHP & MySQL View </h3>
<?php
//- raspiDB 관련 변수 선언 -----------------------------------------------------
$db_host = "localhost";
$db_user = "202213109";
$db_passwd = "10015";
$db_name = "EMB202213109";
$db_port = 3306 ;
//- raspiDB 연결 -----------------------------------------------------------------
echo "(1) DB Connecting -----------------------<br>";
$conn = mysqli_connect( $db_host, $db_user, $db_passwd, $db_name,$db_port );
if(!$conn) {
echo " \tDB Connect : Fail ~!! [ " . mysqli_connect_errno() . " ] \t";
echo mysqli_connect_error() . "<br><br>";
}else {
echo " \tDB Connected. OK~!!<br><br>";
}
//- raspiDB 데이터 조회-------------------------------------------
echo "(2) DB Query ---------------------------------<br>";
$query = "select * from sensor";
$result = mysqli_query($conn, $query);
$rows = mysqli_num_rows($result);
echo "Rows = " . $rows."<br><br>";

if ($result && mysqli_num_rows($result) > 0) {
    // 테이블 헤더 출력
    echo "<table border='1'>
    <tr>
    <th>time</th>
    <th>ultrasonic</th>
    <th>illum</th>
    </tr>";
    while ($row = mysqli_fetch_assoc($result)) {
    echo "<tr>
    <td>" . htmlspecialchars($row["time"]) . "</td>
    <td>" . htmlspecialchars($row["ultrasonic"]) . "</td>
    <td>" . htmlspecialchars($row["illum"]) . "</td>
    </tr>";
    }
    echo "</table>";
    }else{
    echo "0 results";
    }
    //- Disconnect ------------------------------------------------------
    echo "(3) DB Disconnect ----------------------<br>";
    mysqli_free_result($result);
    mysqli_close($connect);
    echo " \tDB Disconnect OK ~ !!<br>";
    ?>
    </body>
    </html>