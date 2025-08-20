# Web Scraper

A simple web scraping tool that extracts content between specified HTML tags from websites.

## Dependencies

- **libCurl** - Required for making HTTP requests

## How It Works

The scraper extracts all content located between specified start and end HTML tags from target websites. All scraped data is saved to a file in the `/build` directory.

## Configuration

### Setup Instructions

1. Edit the `chain.txt` file to configure your scraping job
2. Specify the following parameters in `chain.txt`:
   - **URL(s)**: The website(s) you want to scrape
   - **Start tag(s)**: The opening HTML tag where content extraction begins
   - **End tag(s)**: The closing HTML tag where content extraction ends
   - **Output filename**: Name for the saved data file (optional)

### Example Configuration

An example `chain.txt` file is provided with the project to demonstrate the proper format.

## Usage

1. Configure your scraping parameters in `chain.txt`
2. Run the program
3. Check the `/build` directory for your scraped data

## Important Notes

- **JavaScript-rendered sites**: These may require additional effort to find working start tags, as content might be dynamically loaded
- **Output location**: All scraped data is automatically saved to the `/build` directory
- **Custom filenames**: You can specify a custom output filename in the `chain.txt` configuration file

## Troubleshooting

If you're having trouble with JavaScript-heavy websites, try:
- Inspecting the page source after it fully loads
- Looking for content that appears in the initial HTML rather than dynamically loaded elements
- Using more specific or unique HTML tags as start/end markers
