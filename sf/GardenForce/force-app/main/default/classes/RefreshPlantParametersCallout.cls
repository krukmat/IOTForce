/**
 * @description       : 
 * @author            : Matias Kruk
 * @group             : 
 * @last modified on  : 11-09-2021
 * @last modified by  : Matias Kruk
 * Modifications Log 
 * Ver   Date         Author        Modification
 * 1.0   05-19-2021   Matias Kruk   Initial Version
**/
public with sharing class RefreshPlantParametersCallout {
    private static String generateRandomString(Integer len) {
        final String chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz';
        String randStr = '';
        while (randStr.length() < len) {
           Integer idx = Math.mod(Math.abs(Crypto.getRandomInteger()), chars.length());
           randStr += chars.substring(idx, idx+1);
        }
        return randStr; 
    }
    @AuraEnabled(cacheable=true)
    public static String updatePlantMoist(String plant, Integer parameter) {
        Http http = new Http();
        HttpRequest request = new HttpRequest();
         
        // Provide the Named Credentials
        request.setEndpoint('https://gardenforce.krukforce.com/rest/setup?device_id='+plant+'&moist='+String.valueOf(parameter));
        request.setMethod('GET');        
        HttpResponse response = http.send(request);
        if (response.getStatusCode() == 200) {
            return response.getBody();
        }
        return null;
    }

    @AuraEnabled(cacheable=true)
    public static String updatePlantReadMs(String plant, Integer parameter) {
        Http http = new Http();
        HttpRequest request = new HttpRequest();
         
        // Provide the Named Credentials
        request.setEndpoint('https://gardenforce.krukforce.com/rest/setup?device_id='+plant+'&read_ms='+String.valueOf(parameter));
        request.setMethod('GET');        
        HttpResponse response = http.send(request);
        if (response.getStatusCode() == 200) {
            return response.getBody();
        }
        return null;
    }

    @AuraEnabled(cacheable=true)
    public static String updatePlantFlowMS(String plant, Integer parameter) {
        Http http = new Http();
        HttpRequest request = new HttpRequest();
         
        // Provide the Named Credentials
        request.setEndpoint('https://gardenforce.krukforce.com/rest/setup?device_id='+plant+'&flow_ms='+String.valueOf(parameter));
        request.setMethod('GET');        
        HttpResponse response = http.send(request);
        if (response.getStatusCode() == 200) {
            return response.getBody();
        }
        return null;
    }

    @AuraEnabled(cacheable=true)
    public static String hidrate(String plant) {
        Http http = new Http();
        HttpRequest request = new HttpRequest();
         
        // Provide the Named Credentials
        request.setEndpoint('https://gardenforce.krukforce.com/rest/execute?device_id='+plant+'&rnd='+generateRandomString(10));
        request.setMethod('GET');        
        HttpResponse response = http.send(request);
        if (response.getStatusCode() == 200) {
            return response.getBody();
        }
        return null;
    }

    public static void hidrate(String plant, Integer numberOfTimes){
            for (Integer i = 0; i < numberOfTimes; i++){
                RefreshPlantParametersCallout.hidrate(plant);
            }
    }
}