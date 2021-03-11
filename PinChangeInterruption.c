	if (changed_bits & (1 << PINB4)){
		if (PINB & (1 << PINB4)) {
		// D12 mudou de LOW para HIGH;
		}else {
		// D12 mudou de HIGH para LOW;
		}
	}else{
		if (changed_bits & (1 << PINB3)){
			if (PINB & (1 << PINB3)) {
			// D11 mudou de LOW para HIGH;
			}else {
			// D11 mudou de HIGH para LOW;
			}
		}else{
			if (changed_bits & (1 << PINB2)){
				if (PINB & (1 << PINB2)) {
				// D10 mudou de LOW para HIGH;
				}else {
				// D10 mudou de HIGH para LOW;
				}
			}
		}
	}