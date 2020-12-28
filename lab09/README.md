# Лабораторная работа IX

## Задачи

1. Назначить адреса подсетей:
   1. Подсеть 1: 192.168.x.0 /24
   2. Подсеть 2: 192.168.x+1.0 /24
   3. Подсеть 3: 192.168.x+2.0 /24
2. Настроить поддержку трех виртуальных локальных сетей (VLan 10, 20, 30) на коммутаторе.
3. Настроить маршрутизацию между виртуальными локальными сетями на маршрутизаторе.
4. Выделить и озаглавить на схеме каждую виртуальную локальную сеть.

## Подсети
<table>
	<tr>
		<th>№</th>
		<th>Binary CIDR</th>
		<th>IPv4</th>
		<th>Mask</th>
	</tr>
	<tr>
		<td>1</td>
		<td><b>11000000.10101000.00000110</b>.00000000/24</td>
		<td>192.168.6.0</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>2</td>
		<td><b>11000000.10101000.00000111</b>.00000000/24</td>
		<td>192.168.7.0</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>3</td>
		<td><b>11000000.10101000.00001000</b>.00000000/24</td>
		<td>192.168.8.0</td>
		<td>255.255.255.0</td>
	</tr>
</table>

## Настройка

### Задание 1

На хостах были настроены адреса интерфейсов.

### Задание 2

Switch0
```
Switch>en
Switch#sh vl

VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/1, Fa0/2, Fa0/3, Fa0/4
                                                Fa0/5, Fa0/6, Fa0/7, Fa0/8
                                                Fa0/9, Fa0/10, Fa0/11, Fa0/12
                                                Fa0/13, Fa0/14, Fa0/15, Fa0/16
                                                Fa0/17, Fa0/18, Fa0/19, Fa0/20
                                                Fa0/21, Fa0/22, Fa0/23, Fa0/24
                                                Gig0/1, Gig0/2
1002 fddi-default                     active
1003 token-ring-default               active
1004 fddinet-default                  active
1005 trnet-default                    active

VLAN Type  SAID       MTU   Parent RingNo BridgeNo Stp  BrdgMode Trans1 Trans2
---- ----- ---------- ----- ------ ------ -------- ---- -------- ------ ------
1    enet  100001     1500  -      -      -        -    -        0      0
1002 fddi  101002     1500  -      -      -        -    -        0      0
1003 tr    101003     1500  -      -      -        -    -        0      0
1004 fdnet 101004     1500  -      -      -        ieee -        0      0
1005 trnet 101005     1500  -      -      -        ibm  -        0      0

VLAN Type  SAID       MTU   Parent RingNo BridgeNo Stp  BrdgMode Trans1 Trans2
---- ----- ---------- ----- ------ ------ -------- ---- -------- ------ ------

Remote SPAN VLANs
------------------------------------------------------------------------------

Primary Secondary Type              Ports
------- --------- ----------------- ------------------------------------------
Switch#conf t

Switch(config)#in g0/1
Switch(config-if)#sw m t

Switch(config-if)#in v 10
Switch(config-if)#interface range f0/1-f0/2
Switch(config-if-range)#sw m a
Switch(config-if-range)#sw a v 10

Switch(config-if-range)#in v 20
Switch(config-if)#interface range f0/3-f0/4
Switch(config-if-range)#sw m a
Switch(config-if-range)#sw a v 20

Switch(config-if-range)#in v 30
Switch(config-if)#interface range f0/5-f0/7
Switch(config-if-range)#sw m a
Switch(config-if-range)#sw a v 30

Switch(config-if-range)#ex
Switch(config)#ex

Switch#sh vl

VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/8, Fa0/9, Fa0/10, Fa0/11
                                                Fa0/12, Fa0/13, Fa0/14, Fa0/15
                                                Fa0/16, Fa0/17, Fa0/18, Fa0/19
                                                Fa0/20, Fa0/21, Fa0/22, Fa0/23
                                                Fa0/24, Gig0/1, Gig0/2
10   VLAN0010                         active    Fa0/1, Fa0/2
20   VLAN0020                         active    Fa0/3, Fa0/4
30   VLAN0030                         active    Fa0/5, Fa0/6, Fa0/7
1002 fddi-default                     active
1003 token-ring-default               active
1004 fddinet-default                  active
1005 trnet-default                    active

VLAN Type  SAID       MTU   Parent RingNo BridgeNo Stp  BrdgMode Trans1 Trans2
---- ----- ---------- ----- ------ ------ -------- ---- -------- ------ ------
1    enet  100001     1500  -      -      -        -    -        0      0
10   enet  100010     1500  -      -      -        -    -        0      0
20   enet  100020     1500  -      -      -        -    -        0      0
30   enet  100030     1500  -      -      -        -    -        0      0
1002 fddi  101002     1500  -      -      -        -    -        0      0
1003 tr    101003     1500  -      -      -        -    -        0      0
1004 fdnet 101004     1500  -      -      -        ieee -        0      0
1005 trnet 101005     1500  -      -      -        ibm  -        0      0

VLAN Type  SAID       MTU   Parent RingNo BridgeNo Stp  BrdgMode Trans1 Trans2
---- ----- ---------- ----- ------ ------ -------- ---- -------- ------ ------

Remote SPAN VLANs
------------------------------------------------------------------------------

Primary Secondary Type              Ports
------- --------- ----------------- ------------------------------------------
Switch#ex
```

### Задание 3

Router0
```
Router>en
Router#conf t

Router(config)#in g0/0/0
Router(config-if)#ip ad 192.168.1.1 255.255.255.0
Router(config-if)#no sh

Router(config-if)#in g0/0/0.1
Router(config-subif)#ip ad 192.168.8.254 255.255.255.0
Router(config-subif)#en d 10

Router(config-subif)#in g0/0/0.2
Router(config-subif)#ip ad 192.168.7.254 255.255.255.0
Router(config-subif)#en d 20

Router(config-subif)#in g0/0/0.3
Router(config-subif)#ip ad 192.168.8.254 255.255.255.0
Router(config-subif)#en d 30

Router(config-subif)#ex
Router(config)#ex
Router#ex
```
