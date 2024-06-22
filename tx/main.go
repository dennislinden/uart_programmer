package main

import (
	"encoding/binary"
	"fmt"
	"log"

	"go.bug.st/serial"
)

func main() {
	ports, err := serial.GetPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found!")
	}
	for _, port := range ports {
		fmt.Printf("Found port: %v\n", port)
	}

	mode := &serial.Mode{
		BaudRate: 57600,
		Parity:   serial.NoParity,
		DataBits: 8,
		StopBits: serial.OneStopBit,
	}

	port, err := serial.Open("/dev/ttyUSB0", mode)
	if err != nil {
		log.Fatal(err)
	}
	err = port.SetMode(mode)
	if err != nil {
		log.Fatal(err)
	}
	sendBytes(port, 0x0f8)
}

func sendBytes(port serial.Port, addressOffset int) {
	// First value is ROM Address offset
	const bytelen = 0x0
	var mySlice = []byte{byte(addressOffset), bytelen, 0x1}
	// mySlice = append(mySlice, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1)
	var lenData = len(mySlice)
	mySlice[1] = byte(lenData)

	// var mySlice = []int{0x2}

	for _, i := range mySlice {
		// time.Sleep(200 * time.Millisecond)
		b := make([]byte, 2)
		i64 := uint16(i)
		binary.LittleEndian.PutUint16(b, i64)
		fmt.Printf("%b", b)

		n, err := port.Write([]byte{b[0]})
		if err != nil {
			port.Close()
			log.Fatal(err)

		}
		fmt.Printf("Sent %v bytes\n", n)

	}

	defer port.Close()
	// buff := make([]byte, 100)
	// for {
	// 	n, err := port.Read(buff)
	// 	if err != nil {
	// 		log.Fatal(err)
	// 		break
	// 	}
	// 	if n == 0 {
	// 		fmt.Println("\nEOF")
	// 		break
	// 	}
	// 	fmt.Printf("%v", string(buff[:n]))
	// }
}
