# Лабораторная работа VIII

## Задачи

1. Назначить адреса подсетей:
   1. Подсеть 1: 192.168.x.0 /24
   2. Подсеть 2: 192.168.x+1.0 /24
   3. Подсеть 3: 192.168.x+2.0 /24
   4. Подсеть 4: 192.168.x+3.0 /24
   5. Подсеть 5 (В задаче III): 192.168.x+10.0 /24
2. Настроить динамическую маршрутизацию в прилагаемом .pkt файле на стенде I через протокол RIPv2 так, чтобы пинг любым хостом или маршрутизатором любого другого хоста или маршрутизатора был успешным. Представить отдельным .pkt файлом.
3. Настроить динамическую маршрутизацию в сети в прилагаемом .pkt файле на стенде II через протокол OSPF так, чтобы пинг любым хостом или маршрутизатором любого другого хоста или маршрутизатора был успешным. Разделить при этом сеть на области OSPF в соответствии со схемой. Выполнить указания в лабораторной работе. Представить отдельным .pkt файлом.


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
	<tr>
		<td>4</td>
		<td><b>11000000.10101000.00001001</b>.00000000/24</td>
		<td>192.168.9.0</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>5</td>
		<td><b>11000000.10101000.00010000.00000000/24</td>
		<td>192.168.16.0</td>
		<td>255.255.255.0</td>
	</tr>
</table>

## Настройка

### Задание 1

Настройка маршрутизаторов:

Router0
```
Router>en
Router#conf t
Router(config)#in G 0/0/0
Router(config-if)#ip ad 192.168.6.254 255.255.255.0
Router(config-if)#ex
Router(config)#in S 0/1/0
Router(config-if)#ip ad 192.168.8.254 255.255.255.0
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router1
```
Router>en
Router#conf t
Router(config)#in G 0/0/0
Router(config-if)#ip ad 192.168.7.254 255.255.255.0
Router(config-if)#ex
Router(config)#in S 0/1/0
Router(config-if)#ip ad 192.168.9.254 255.255.255.0
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router2
```
Router>en
Router#conf t
Router(config)#in S 0/1/0
Router(config-if)#ip ad 192.168.8.253 255.255.255.0
Router(config-if)#ex
Router(config)#in S 0/1/1
Router(config-if)#ip ad 192.168.9.253 255.255.255.0
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router7
```
Router>en
Router#conf t
Router(config)#in G 0/0/0
Router(config-if)#ip ad 192.168.6.254 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ad 192.168.16.254 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router8
```
Router>en
Router#conf t
Router(config)#in G 0/0/0
Router(config-if)#ip ad 192.168.7.254 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ad 192.168.16.253 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router9
```
Router>en
Router#conf t
Router(config)#in G 0/0/0
Router(config-if)#ip ad 192.168.8.254 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ad 192.168.16.252 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router10
```
Router>en
Router#conf t
Router(config)#in G 0/0/0
Router(config-if)#ip ad 192.168.9.254 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ad 192.168.16.251 255.255.255.0
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ex
Router#ex
```

На хостах были настроены адреса интерфейсов и адреса шлюзов по умолчанию.

### Задание 2

Router0
```
Router>en
Router#sh ip p
Router#sh ip ri d
Router#conf t
Router(config)#ro r
Router(config-router)#ne 192.168.8.0
Router(config-router)#v 2
Router(config-router)#ex
Router(config)#ex
Router#ex
```

Router1
```
Router>en
Router#sh ip p
Router#sh ip ri d
Router#conf t
Router(config)#ro r
Router(config-router)#ne 192.168.9.0
Router(config-router)#v 2
Router(config-router)#ex
Router(config)#ex
Router#ex
```

Router2
```
Router>en
Router#sh ip p
Router#sh ip ri d
Router#conf t
Router(config)#ro r
Router(config-router)#ne 192.168.8.0
Router(config-router)#ne 192.168.9.0
Router(config-router)#v 2
Router(config-router)#ex
Router(config)#ex
Router#ex
```

Router0
```
Router>en
Router#sh ip ro
Codes: L - local, C - connected, S - static, R - RIP, M - mobile, B - BGP
       D - EIGRP, EX - EIGRP external, O - OSPF, IA - OSPF inter area
       N1 - OSPF NSSA external type 1, N2 - OSPF NSSA external type 2
       E1 - OSPF external type 1, E2 - OSPF external type 2, E - EGP
       i - IS-IS, L1 - IS-IS level-1, L2 - IS-IS level-2, ia - IS-IS inter area
       * - candidate default, U - per-user static route, o - ODR
       P - periodic downloaded static route

Gateway of last resort is not set

     192.168.6.0/24 is variably subnetted, 2 subnets, 2 masks
C       192.168.6.0/24 is directly connected, GigabitEthernet0/0/0
L       192.168.6.254/32 is directly connected, GigabitEthernet0/0/0
R    192.168.7.0/24 [120/2] via 192.168.8.253, 00:00:13, Serial0/1/0
     192.168.8.0/24 is variably subnetted, 2 subnets, 2 masks
C       192.168.8.0/24 is directly connected, Serial0/1/0
L       192.168.8.254/32 is directly connected, Serial0/1/0
R    192.168.9.0/24 [120/1] via 192.168.8.253, 00:00:13, Serial0/1/0

Router#ex
```

### Задание 3

Router7
```
Router>en
Router#sh ip ospf interface

Router#conf t
Router(config)#route ospf 1
Router(config-router)#network 192.168.6.0 0.0.0.255 area 1
Router(config-router)#network 192.168.16.0 0.0.0.255 area 0
Router(config-router)#ex
Router(config)#in G 0/0/0
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router8
```
Router>en
Router#sh ip ospf interface

Router#conf t
Router(config)#route ospf 1
Router(config-router)#network 192.168.7.0 0.0.0.255 area 2
Router(config-router)#network 192.168.16.0 0.0.0.255 area 0
Router(config-router)#ex
Router(config)#in G 0/0/0
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router9
```
Router>en
Router#sh ip ospf interface

Router#conf t
Router(config)#route ospf 1
Router(config-router)#network 192.168.8.0 0.0.0.255 area 3
Router(config-router)#network 192.168.16.0 0.0.0.255 area 0
Router(config-router)#ex
Router(config)#in G 0/0/0
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router10
```
Router>en
Router#sh ip ospf interface

Router#conf t
Router(config)#route ospf 1
Router(config-router)#network 192.168.9.0 0.0.0.255 area 4
Router(config-router)#network 192.168.16.0 0.0.0.255 area 0
Router(config-router)#ex
Router(config)#in G 0/0/0
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#in G 0/0/1
Router(config-if)#ip ospf authentication-key key
Router(config-if)#ex
Router(config)#ex
Router#ex
```

Router7
```
Router>en
Router#sh ip ro
Codes: L - local, C - connected, S - static, R - RIP, M - mobile, B - BGP
D - EIGRP, EX - EIGRP external, O - OSPF, IA - OSPF inter area
N1 - OSPF NSSA external type 1, N2 - OSPF NSSA external type 2
E1 - OSPF external type 1, E2 - OSPF external type 2, E - EGP
i - IS-IS, L1 - IS-IS level-1, L2 - IS-IS level-2, ia - IS-IS inter area
* - candidate default, U - per-user static route, o - ODR
P - periodic downloaded static route

Gateway of last resort is not set

192.168.6.0/24 is variably subnetted, 2 subnets, 2 masks
C       192.168.6.0/24 is directly connected, GigabitEthernet0/0/0
L       192.168.6.254/32 is directly connected, GigabitEthernet0/0/0
O IA 192.168.7.0/24 [110/2] via 192.168.16.253, 00:27:05, GigabitEthernet0/0/1
O IA 192.168.8.0/24 [110/2] via 192.168.16.252, 00:27:05, GigabitEthernet0/0/1
O IA 192.168.9.0/24 [110/2] via 192.168.16.251, 00:27:05, GigabitEthernet0/0/1
192.168.16.0/24 is variably subnetted, 2 subnets, 2 masks
C       192.168.16.0/24 is directly connected, GigabitEthernet0/0/1
L       192.168.16.254/32 is directly connected, GigabitEthernet0/0/1

Router#ex
```
