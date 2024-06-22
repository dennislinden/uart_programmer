package main

import "fmt"
func main(){
	maxVal := 999
	fmt.Println("hundreds")
	for i := 0; i < maxVal; i++{

		hundreds := i / 100 % 10
		m := i % 16
		printAddress(i, m)
		printContents(hundreds)
	}

	fmt.Println("\n\ntens\n")
	for i := 0; i < maxVal; i++{

		tens := i / 10 % 10
		m := i % 16

		printAddress(i, m)
		printContents(tens)

	}
	fmt.Println("\n\nones\n")
	for i := 0; i < maxVal; i++{
		m := i % 16
		printAddress(i, m)

		ones := i % 10
		printContents(ones)
	}
}
func printAddress(i int, m int){
		//fmt.Printf("check: %x\n", i)
		if m == 0 {
			fmt.Printf("\n%x ", i)

		}
		//} else if m == 0 && (i / 10 % 10) == 0{ 
		//	fmt.Printf("\n%x: ", i)
		//} else if m == 0 && (i / 100 % 10) == 0{ 
		//	fmt.Printf("\n%x: ", i)
		//}

}

func printContents(data int){

		segZero := 0x3f
		segOne := 0x06
		segTwo := 0x5b
		segThree := 0x4f
		segFour := 0x66
		segFive := 0x6D
		segSix := 0x7D
		segSeven := 0x7
		segEight := 0x7f
		segNine:= 0x6F

		switch data{
		case 0:
			fmt.Printf("%x ",segZero)
		case 1:
			fmt.Printf("0%x ",segOne)
		case 2:
			fmt.Printf("%x ",segTwo)
		case 3:
			fmt.Printf("%x ",segThree)
		case 4:
			fmt.Printf("%x ",segFour)
		case 5:
			fmt.Printf("%x ",segFive)
		case 6:
			fmt.Printf("%x ",segSix)
		case 7:
			fmt.Printf("0%x ",segSeven)
		case 8:
			fmt.Printf("%x ",segEight)
		case 9:
			fmt.Printf("%x ",segNine)
		case 10:
			fmt.Printf("%x ",segEight)
		case 11:
			fmt.Printf("%x ",segEight)
		case 12:
			fmt.Printf("%x ",segEight)
		case 13:
			fmt.Printf("%x ",segEight)
		case 14:
			fmt.Printf("%x ",segEight)
		case 15:
			fmt.Printf("%x ",segEight)
		case 16:
			fmt.Printf("%x ",segEight)
		}
}
