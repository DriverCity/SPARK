class PaymentException(Exception): pass

def validate(payment_method_type, payment_method_information):
    if payment_method_type == 'SERVICE_1' and payment_method_information['paymentReceipt'] == 'valid_test_hash':
        pass
    else:
        # TODO
        raise PaymentException({'paymentMethodType': payment_method_type, 'reason': 'some reason'})