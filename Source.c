// Joseph Fabel
// CS 2600
// Programming Project 2
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
	int terms;
	int* exponents;
	float* coefficients;
} Polynomial;

Polynomial addPolynomials(Polynomial poly1, Polynomial poly2);
Polynomial subtractPolynomials(Polynomial poly1, Polynomial poly2);
void removeZeroTerms(Polynomial poly, int* polynomialTerms);
int combinedTotalTerms(Polynomial poly1, Polynomial poly2);
void sortPolynomial(Polynomial poly);
Polynomial inputPolynomialFromUser();
void printPolynomial(Polynomial poly);
Polynomial allocatePolynomial(int terms);
void freePolynomial(Polynomial poly);

int main()
{
	// errors if first character is negative

	printf("Polynomials should be entered in the format 4x^0 + x^1 - 2x^2 ...\n\n");
	printf("Please enter the first polynomial: ");
	Polynomial poly1 = inputPolynomialFromUser();

	printf("Please enter a second polynomial: ");
	Polynomial poly2 = inputPolynomialFromUser();

	// Sort both polynomials so that they can be used easier
	sortPolynomial(poly1);
	sortPolynomial(poly2);

	// Perform the user's desired operation(s)
	int inputChoice = 0;
	int successfulScan = 0;
	printf("Please choose an option (1-4):\n(1) Addition\n(2) Subtraction\n(3) Enter two new polynomials\n(4) Exit\n");
		
	// Loops as long as a value is successfully scanned
	successfulScan = scanf("%d", &inputChoice);
	while (successfulScan > 0)
	{
		// Separate output with a newline
		printf("\n");

		// Perform the operation
		if (inputChoice == 1)
		{
			// Add the two polynomials
			Polynomial resultant = addPolynomials(poly1, poly2);

			printf("Polynomial 1 + Polynomial 2 = ");
			printPolynomial(resultant);
		}
		else if (inputChoice == 2)
		{
			// Subtract the two polynomials
			Polynomial resultant = subtractPolynomials(poly1, poly2);

			printf("Polynomial 1 - Polynomial 2 = ");
			printPolynomial(resultant);
		}
		else if (inputChoice == 3)
		{
			// Get 2 new polynomials

			// First scan any unused characters so that the new polynomial can be read
			successfulScan = scanf("%*c");
			while (successfulScan) successfulScan = scanf("%*c");
			
			// Free both polynomials before getting their new values
			freePolynomial(poly1);
			freePolynomial(poly2);

			printf("Please enter the first polynomial: ");
			poly1 = inputPolynomialFromUser();

			printf("Please enter a second polynomial: ");
			poly2 = inputPolynomialFromUser();

			// Sort both polynomials so that they can be used easier
			sortPolynomial(poly1);
			sortPolynomial(poly2);
		}
		else if (inputChoice == 4)
		{
			// Exit the program
			exit(0);
		}
		else
		{
			// Remind the user that the option must be bounded between 1 and 4
			printf("Please enter an option between 1 and 4.\n");
		}

		// Promp for a new input value
		printf("Please choose an option (1-4):\n(1) Addition\n(2) Subtraction\n(3) Enter two new polynomials\n(4) Exit\n");
		successfulScan = scanf("%d", &inputChoice);
	}

	// This will only run if successfulScan <= 0, which signifies an error
	printf("\nERROR - Failed to read option from user!\n");
	return 1;
}

// Adds two polynomials together in ascending exponent order and returns the result
Polynomial addPolynomials(Polynomial poly1, Polynomial poly2)
{
	// Allocate memory for the resultant polynomial
	Polynomial resultant = allocatePolynomial(combinedTotalTerms(poly1, poly2));
	int coeffeciant = 0, poly1Term = 0, poly2Term = 0;

	for (int i = 0; i < resultant.terms; i++)
	{
		// It is known that the 2 polynomials are sorted. Therefore there are three options:
		// Both terms have the same exponent
		if (*(poly1.exponents + poly1Term) == *(poly2.exponents + poly2Term))
		{
			*(resultant.exponents + i) = *(poly1.exponents + poly1Term);
			*(resultant.coefficients + i) = *(poly1.coefficients + poly1Term) + *(poly2.coefficients + poly2Term);

			poly1Term++;
			poly2Term++;
		} // The next unused term in poly1 is lower than the next unused term in poly2
		else if (poly2Term >= poly2.terms || poly1Term < poly1.terms && *(poly1.exponents + poly1Term) < *(poly2.exponents + poly2Term))
		{
			// The term is just the term from poly1
			*(resultant.exponents + i) = *(poly1.exponents + poly1Term);
			*(resultant.coefficients + i) = *(poly1.coefficients + poly1Term);

			poly1Term++;
		}
		else // The next unused term in poly2 is lower than the next unused term in poly1
		{
			// The term is just the term from poly2
			*(resultant.exponents + i) = *(poly2.exponents + poly2Term);
			*(resultant.coefficients + i) = *(poly2.coefficients + poly2Term);

			poly2Term++;
		}
	}

	removeZeroTerms(resultant, &resultant.terms);

	// Note that the resultant is already sorted!
	return resultant;
}

// Subtracts poly2 from poly1 and returns the result
Polynomial subtractPolynomials(Polynomial poly1, Polynomial poly2)
{
	// Allocate memory for the resultant polynomial
	Polynomial resultant = allocatePolynomial(combinedTotalTerms(poly1, poly2));
	int coeffeciant = 0, poly1Term = 0, poly2Term = 0;

	for (int i = 0; i < resultant.terms; i++)
	{
		// It is known that the 2 polynomials are sorted. Therefore there are three options:
		// Both terms have the same exponent
		if (*(poly1.exponents + poly1Term) == *(poly2.exponents + poly2Term))
		{
			*(resultant.exponents + i) = *(poly1.exponents + poly1Term);
			*(resultant.coefficients + i) = *(poly1.coefficients + poly1Term) - *(poly2.coefficients + poly2Term);

			poly1Term++;
			poly2Term++;
		} // The next unused term in poly1 is lower than the next unused term in poly2
		else if (poly2Term >= poly2.terms || poly1Term < poly1.terms && *(poly1.exponents + poly1Term) < *(poly2.exponents + poly2Term))
		{
			// The term is just the term from poly1
			*(resultant.exponents + i) = *(poly1.exponents + poly1Term);
			*(resultant.coefficients + i) = *(poly1.coefficients + poly1Term);

			poly1Term++;
		}
		else // The next unused term in poly2 is lower than the next unused term in poly1
		{
			// The term is just the term from poly2
			*(resultant.exponents + i) = *(poly2.exponents + poly2Term);
			*(resultant.coefficients + i) = -*(poly2.coefficients + poly2Term);

			poly2Term++;
		}
	}

	removeZeroTerms(resultant, &resultant.terms);

	// Note that the resultant is already sorted!
	return resultant;
}

void removeZeroTerms(Polynomial poly, int* polynomialTerms)
{
	int requiredTerms = poly.terms;

	// Find how many terms are actually required for the polynomial
	for (int i = 0; i < poly.terms; i++)
	{
		// See if coefficient is approximately 0
		if (abs(*(poly.coefficients + i)) < 0.00001) requiredTerms--;
	}

	// If the polynomial should be shrunk, shrink it!
	if (requiredTerms == 0)
	{
		// Free up the allocated memory
		free(poly.coefficients);
		poly.coefficients = NULL;
		
		free(poly.exponents);
		poly.exponents = NULL;

		*polynomialTerms = 0; // poly.terms is not a pointer so this pointer is necessary to get this value to be persistent
	}
	else if (poly.terms > requiredTerms)
	{
		// Store the old coefficient array in a temporary variable and allocate a new array to the coefficient array
		float* tempCoefficients = poly.coefficients;
		poly.coefficients = (float*)malloc(requiredTerms * sizeof(float));

		// Same thing for exponents
		int* tempExponents = poly.exponents;
		poly.exponents = (int*)malloc(requiredTerms * sizeof(int));

		int currentTerm = 0;
		for (int i = 0; i < poly.terms; i++)
		{
			// If the coefficient is not zero, copy it over to the new polynomial
			if (abs(*(tempCoefficients + i)) > 0.00001)
			{
				*(poly.coefficients + currentTerm) = *(tempCoefficients + i);
				*(poly.exponents + currentTerm) = *(tempExponents + i);

				currentTerm++;
			}
		}

		// Free up the temp array now that it has been used
		free(tempCoefficients);
		free(tempExponents);
		
		// Set the polynomials number of terms
		*polynomialTerms = requiredTerms;
	}

}

// Determines how many terms will be used if two polynomials are going to be combined.
int combinedTotalTerms(Polynomial poly1, Polynomial poly2)
{
	int numTerms = 0, poly1Term = 0, poly2Term = 0;

	// Loop until both polynomials have had all there terms analyzed, increasing numTerms by 1 each loop
	for (numTerms = 0; poly1Term < poly1.terms || poly2Term < poly2.terms; numTerms++)
	{
		if (poly1Term >= poly1.terms)
		{
			// Increase poly2 counter as it is the only one left to check
			poly2Term++;
		}
		else if (poly2Term >= poly2.terms)
		{
			// Increase poly1 counter as it is the only one left to check
			poly1Term++;
		}
		else if (*(poly1.exponents + poly1Term) == *(poly2.exponents + poly2Term))
		{
			// Both polynomials had the same exponent at that term, increase both counters by 1
			poly1Term++;
			poly2Term++;
		}
		else if (*(poly1.exponents + poly1Term) < *(poly2.exponents + poly2Term))
		{
			// The next term in poly1 was a lower exponent than the next in poly2
			poly1Term++;
		}
		else
		{
			// The next term in poly2 was a lower exponent than the next in poly1
			poly2Term++;
		}
	}

	return numTerms;
}

// Sorts the polynomial so that the terms are arranged in ascending order of exponents
void sortPolynomial(Polynomial poly)
{
	int smallestIndex = 0;
	int tempValue = 0;

	// Selection sort
	for (int i = 0; i < poly.terms - 1; i++)
	{
		smallestIndex = i;

		// Find the smalles exponent and keep its index
		for (int j = i + 1; j < poly.terms; j++) if (*(poly.exponents + j) < *(poly.exponents + i)) smallestIndex = j;
		
		// Swap the element at i with the greatest element, as long as it is not already there
		if (i != smallestIndex)
		{
			// Swap exponents
			tempValue = *(poly.exponents + i);
			*(poly.exponents + i) = *(poly.exponents + smallestIndex);
			*(poly.exponents + smallestIndex) = tempValue;

			// Swap Coefficients
			tempValue = *(poly.coefficients + i);
			*(poly.coefficients + i) = *(poly.coefficients + smallestIndex);
			*(poly.coefficients + smallestIndex) = tempValue;
		}
	}
}

// Takes a string from the user as input, creates a polynomial from that string, and then returns the polynomial.
Polynomial inputPolynomialFromUser()
{
	Polynomial poly;

	// Create a char array that will be used to store the user input
	// Input limited to 100 characters
	char userInput[100], *currentCoefficient = NULL;
	int numTerms = 0, currentTerm = 0, lastDigitIndex, firstDigitIndex;

	// Get the user input to the end of the line
	scanf("%\[^\n]%*c", userInput);

	// Scan the user input for all xs, as they connotate a new term
	for (int i = 0; i < strlen(userInput); i++)
	{
		if (*(userInput + i) == 'x') numTerms++;
	}

	poly = allocatePolynomial(numTerms);

	// Scan the user input for all xs, as they connotate a new term
	// This time the values will be stored
	for (int i = 0; i < strlen(userInput); i++)
	{
		// A new term was found, store the necessary values
		if (*(userInput + i) == 'x')
		{
			// If there was no coefficient provided, it is implied to be 1
			if (i == 0 || !isdigit(*(userInput + i - 1)))
			{
				*(poly.coefficients + currentTerm) = 1;
				firstDigitIndex = i; // This allows for the sign to be checked
			}

			// Otherwise, load the given coefficient into the polynomial
			else
			{
				lastDigitIndex = i - 1;
				for (firstDigitIndex = lastDigitIndex; firstDigitIndex > 0; firstDigitIndex--)
				{
					if (*(userInput + firstDigitIndex) == '.' || isdigit(*(userInput + firstDigitIndex))) continue;
					else break;
				}

				// Get the substring of the desired number portion
				// Create a temporary array that copies the portion of the user input desired
				currentCoefficient = (char*)malloc(lastDigitIndex - firstDigitIndex + 2); // Make the array big enough for all characters and numm-terminator
				memcpy(currentCoefficient, userInput + firstDigitIndex, lastDigitIndex - firstDigitIndex + 1); // Copy the portion of the string to the currentCoefficient

				// See if the memory was successfully allocated
				if (currentCoefficient == NULL)
				{
					printf("\nERROR - coefficient not found!\n");
					exit(2);
				}
				// Add the null terminator to the string
				*(currentCoefficient + lastDigitIndex - firstDigitIndex + 1) = '\0';


				// Put the entered number into the polynomial
				*(poly.coefficients + currentTerm) = atof(currentCoefficient);
			}

			// Check for a positive or negative sign between this number and the previous number and set the sign accordingly
			for (lastDigitIndex = firstDigitIndex; lastDigitIndex > 0; lastDigitIndex--) // Look in reverse order from the first digit of the coefficient
			{
				if (*(userInput + lastDigitIndex) == '+') break; // The entered number was positive
				else if (*(userInput + lastDigitIndex) == '-') 
				{
					*(poly.coefficients + currentTerm) *= -1; // The entered number was negative
					break;
				}
				else if (isdigit(*(userInput + lastDigitIndex))) // Error, no sign was provided (reached the exponent of the previous polynomial)
				{
					printf("\nERROR - no + or - sign found between polynomials!\n");
					exit(3);
				}
			}

			// Repeat this process again to get the exponent
			firstDigitIndex = i + 2;
			for (lastDigitIndex = firstDigitIndex; lastDigitIndex < strlen(userInput); lastDigitIndex++)
			{
				if (isdigit(*(userInput + lastDigitIndex))) continue;
				else break;
			}

			// Get the substring for the exponent
			free(currentCoefficient);
			currentCoefficient = (char*)malloc(lastDigitIndex - firstDigitIndex + 2);
			memcpy(currentCoefficient, userInput + firstDigitIndex, lastDigitIndex - firstDigitIndex + 1);

			if (currentCoefficient == NULL)
			{
				printf("\nERROR - exponent not found!\n");
				exit(4);
			}
			*(currentCoefficient + lastDigitIndex - firstDigitIndex + 1) = '\0';

			*(poly.exponents + currentTerm) = atoi(currentCoefficient);
			currentTerm++;

		}
	}

	// Sort the polynomial before returning it so that using it is easier
	sortPolynomial(poly);

	return poly;
}

// Prints out the specified polynomial to the console
void printPolynomial(Polynomial poly)
{
	if (poly.terms == 0)
	{
		// No terms in polynomial, just print 0
		printf("0");
	}

	for (int i = 0; i < poly.terms; i++)
	{
		// Display the polynomial with a space between the sign and the number
		if (i > 0) printf("%c ", *(poly.coefficients + i) >= 0 ? '+' : '-');
		printf("%.2fx^%d ",  *(poly.coefficients + i) > 0 ? *(poly.coefficients + i) : -*(poly.coefficients + i), *(poly.exponents + i));
	}

	printf("\n");
}

// Allocates memory for a new polynomial with the specified number of terms
 Polynomial allocatePolynomial(int terms)
{
	// allocate memory to a new polynomial
	Polynomial poly;
	poly.terms = terms;
	poly.exponents = (int*)malloc(poly.terms * sizeof(int));
	poly.coefficients = (float*)malloc(poly.terms * sizeof(float));

	// Check to see if memory was allocated
	if (poly.exponents == NULL || poly.coefficients == NULL)
	{
		printf("\nERROR - Not enough memory!\n");
		exit(1);
	}

	return poly;
}

 // Frees the polynomial from memory so that you don't waste memory if reallocating a polynomial
 void freePolynomial(Polynomial poly)
 {
	 free(poly.coefficients);
	 free(poly.exponents);

	 poly.coefficients = NULL;
	 poly.exponents = NULL;

	 poly.terms = 0;
 }