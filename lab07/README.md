# Лабораторная работа VII

## Задачи

1. Разделить сеть на подсети в соответствии с системой адресации IPv4.

   Выделить достаточно адресов для размещения x+20 хостов в подсетяx 1 и 2,  x+10 хостов в подсети 3, по 2 адреса для двухточечных соединений между маршрутизаторами.

   где x - Ваш номер по списку в ЭУ
2. Настроить статическую маршрутизацию  так, чтобы пинг любым хостом или маршрутизатором любого другого хоста или маршрутизатора был успешным. 
3. Выделить маршрутизаторам IPv6 адреса формата 2001:x+y::z/64 где x - Ваш номер по списку в ЭУ, y - порядковый номер подсети, z - порядковый номер интерфейса

4. Установить автоконфигурирование IPv6 (SLAAC) для интерфейсов хостов в подсетях 1 и 2, автоконфигурирование SLAAC + DHCPv6 в подсети 3

5. Настроить статическую маршрутизацию так, чтобы пинг любым хостом или маршрутизатором любого другого хоста или маршрутизатора с использованием IPv6 адреса  был успешным

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
		<td><b>11000000.10101000.00000110.000</b>00000/27</td>
		<td>192.168.6.0</td>
		<td>255.255.255.224</td>
	</tr>
	<tr>
		<td>2</td>
		<td><b>11000000.10101000.00000110.001</b>00000/27</td>
		<td>192.168.6.32</td>
		<td>255.255.255.224</td>
	</tr>
	<tr>
		<td>3</td>
		<td><b>11000000.10101000.00000110.010</b>00000/27</td>
		<td>192.168.6.64</td>
		<td>255.255.255.224</td>
	</tr>
	<tr>
		<td>4</td>
		<td><b>11000000.10101000.00000110.011000</b>00/30</td>
		<td>192.168.6.96</td>
		<td>255.255.255.252</td>
	</tr>
	<tr>
		<td>5</td>
		<td><b>11000000.10101000.00000110.011001</b>00/30</td>
		<td>192.168.6.100</td>
		<td>255.255.255.252</td>
	</tr>
	<tr>
		<td>6</td>
		<td><b>11000000.10101000.00000110.011010</b>00/30</td>
		<td>192.168.6.104</td>
		<td>255.255.255.252</td>
	</tr>
</table>

## Настройка
поменяем порты, включим все маршрутизаторы

Router1
```
Router>en
Router#conf t
Router(config)#interface GigabitEthernet0/0/0
Router(config-if)#ip address 192.168.6.30 255.255.255.224
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:7::1/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#interface GigabitEthernet0/0/1
Router(config-if)#ip address 192.168.6.62 255.255.255.224
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:8::1/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#interface vlan 1
Router(config-if)#ip address 192.168.6.97 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:10::1/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ip dhcp pool pool1
Router(dhcp-config)#network 192.168.6.0 255.255.255.224
Router(dhcp-config)#default-router 192.168.6.30
Router(dhcp-config)#ex
Router(config)#ip dhcp pool pool2
Router(dhcp-config)#network 192.168.6.32 255.255.255.224
Router(dhcp-config)#default-router 192.168.6.62
Router(dhcp-config)#ex
Router(config)#ip route 192.168.6.64 255.255.255.224 192.168.6.98
Router(config)#ip route 192.168.6.100 255.255.255.252 192.168.6.98
Router(config)#ip route 192.168.6.104 255.255.255.252 192.168.6.98
Router(config)#ipv6 route 2001:9::/64 2001:10::2
Router(config)#ipv6 route 2001:11::/64 2001:10::2
Router(config)#ipv6 route 2001:12::/64 2001:10::2
Router(config)#ipv6 unicast-routing
Router(config)#ex
Router#ex
```

Router2
```
Router>en
Router#conf t
Router(config)#interface GigabitEthernet0/0/0
Router(config-if)#ip address 192.168.6.98 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:10::2/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#interface Serial0/1/0
Router(config-if)#ip address 192.168.6.101 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:11::1/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ip route 192.168.6.0 255.255.255.224 192.168.6.97
Router(config)#ip route 192.168.6.32 255.255.255.224 192.168.6.97
Router(config)#ip route 192.168.6.64 255.255.255.224 192.168.6.102
Router(config)#ip route 192.168.6.104 255.255.255.252 192.168.6.102
Router(config)#ipv6 route 2001:7::/64 2001:10::1
Router(config)#ipv6 route 2001:8::/64 2001:10::1
Router(config)#ipv6 route 2001:9::/64 2001:11::2
Router(config)#ipv6 route 2001:12::/64 2001:11::2
Router(config)#ipv6 unicast-routing
Router(config)#ex
Router#ex
```

Router3
```
Router>en
Router#conf t
Router(config)#interface Serial0/1/0
Router(config-if)#ip address 192.168.6.102 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:11::2/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#interface GigabitEthernet0/0/0
Router(config-if)#ip address 192.168.6.105 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:12::1/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ip route 192.168.6.0 255.255.255.224 192.168.6.101
Router(config)#ip route 192.168.6.32 255.255.255.224 192.168.6.101
Router(config)#ip route 192.168.6.64 255.255.255.224 192.168.6.106
Router(config)#ip route 192.168.6.96 255.255.255.252 192.168.6.101
Router(config)#ipv6 route 2001:7::/64 2001:11::1
Router(config)#ipv6 route 2001:8::/64 2001:11::1
Router(config)#ipv6 route 2001:9::/64 2001:12::2
Router(config)#ipv6 route 2001:10::/64 2001:11::1
Router(config)#ipv6 unicast-routing
Router(config)#ex
Router#ex
```

Router4
```
Router>en
Router#conf t
Router(config)#interface GigabitEthernet0/0/0
Router(config-if)#ip address 192.168.6.94 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:9::1/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#interface vlan 1
Router(config-if)#ip address 192.168.6.106 255.255.255.252
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 address 2001:12::2/64
Router(config-if)#no sh
Router(config-if)#ex
Router(config)#ip route 192.168.6.0 255.255.255.224 192.168.6.105
Router(config)#ip route 192.168.6.32 255.255.255.224 192.168.6.105
Router(config)#ip route 192.168.6.96 255.255.255.252 192.168.6.105
Router(config)#ip route 192.168.6.100 255.255.255.252 192.168.6.105
Router(config)#ipv6 route 2001:7::/64 2001:12::1
Router(config)#ipv6 route 2001:8::/64 2001:12::1
Router(config)#ipv6 route 2001:10::/64 2001:12::1
Router(config)#ipv6 route 2001:11::/64 2001:12::1
Router(config)#ipv6 unicast-routing
Router(config)#ex
Router#ex
```
