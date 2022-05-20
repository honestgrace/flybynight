// https://www.youtube.com/watch?v=-E3G9lgUthw
// notepad %userprofile%\.npmrc
// npm login
// npm install
// npx playwright test
// npx playwright test --headed --browser chromium

import {test, expect} from '@playwright/test'

test.describe('first test', async () => {
    test("Google test", async ({page}) => {
        await page.goto('https://www.google.com/')
        await page.type('input[name="q"]', 'playwrite')
        await page.click('input[name="btnK"]')
        await page.waitForNavigation()
        const title = await page.title()
        expect(title).toBe('playwrite - Google Search')
        await page.screenshot({path: 'google.png', fullPage: true})
    });
});