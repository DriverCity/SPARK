class PaymentException(Exception): pass


def is_test_payment(payment_data, test_type, test_receipt):

    if test_type is None or test_receipt is None:
        return False

    return payment_data['paymentMethodInformation']['paymentMethodType'] == test_type\
           and payment_data['paymentMethodInformation']['paymentReceipt'] == test_receipt


def validate(payment_data):
    if True == True:
        # Depending on payment type, payment is validated. Further payment validation is not in
        # the scope of this project.
        raise PaymentException()